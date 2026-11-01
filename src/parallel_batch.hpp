#pragma once
// Batch of independent thermal solves: serial vs bounded worker pool.
// Same cases and same solver in both versions; results stay in input order.
// Generator is deterministic (fixed seed 42), with varied positive k and q.
#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>
#include "fea.hpp"

namespace wb {

struct BatchCase { double k = 10.0, q = 10000.0; };

inline std::vector<BatchCase> generate_batch(int count, std::uint32_t seed = 42) {
    // Simple LCG, fixed seed: reproducible on any platform, no <random> drift.
    std::uint32_t s = seed;
    auto next = [&]() { s = s * 1664525u + 1013904223u; return s / 4294967296.0; };
    std::vector<BatchCase> out;
    for (int i = 0; i < count; ++i) {
        BatchCase c;
        c.k = 5.0 + 15.0 * next();    // 5..20 W/(m K), always positive
        c.q = 5000.0 + 15000.0 * next();  // 5000..20000 W/m^3, always positive
        out.push_back(c);
    }
    return out;
}

// One scalar per case so serial/parallel agreement is easy to check exactly:
// the temperature at the rod midpoint (node average for even mesh).
inline double case_scalar(const BatchCase& c, int mesh = 40) {
    FeaResult r = solve_fea(mesh, 1.0, 1e-4, c.k, c.q, 100.0, 20.0);
    return 0.5 * (r.t[mesh / 2] + r.t[mesh / 2 + 1]);
}

inline std::vector<double> run_serial(const std::vector<BatchCase>& cases, int mesh) {
    std::vector<double> out(cases.size());
    for (std::size_t i = 0; i < cases.size(); ++i) out[i] = case_scalar(cases[i], mesh);
    return out;
}

inline std::vector<double> run_parallel(const std::vector<BatchCase>& cases, int mesh, int workers) {
    if (workers < 1) workers = 1;
    std::vector<double> out(cases.size());
    std::atomic<std::size_t> next{0};
    auto worker = [&]() {
        for (;;) {
            std::size_t i = next.fetch_add(1);
            if (i >= cases.size()) break;
            out[i] = case_scalar(cases[i], mesh);  // each solve independent
        }
    };
    std::vector<std::thread> pool;
    for (int w = 0; w < workers; ++w) pool.emplace_back(worker);
    for (auto& t : pool) t.join();
    return out;
}

}  // namespace wb
