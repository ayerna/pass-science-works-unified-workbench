// Unified Engineering Workbench: C++17 core CLI.
// Usage: workbench fea | batch | backprop | validate-stl <file> | validate-csv <file> | mock | all
// All output paths are relative to the working directory (run from project root).
#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "backprop.hpp"
#include "fea.hpp"
#include "parallel_batch.hpp"
#include "parsers.hpp"

using namespace wb;

static void write_fea_csv(const FeaResult& r, const std::string& path) {
    std::ofstream out(path);
    out << "x_m,temperature_C,temperature_exact_C\n";
    for (std::size_t i = 0; i < r.x.size(); ++i)
        out << r.x[i] << "," << r.t[i] << "," << exact_temperature(r.x[i], 1.0, 10.0, 10000.0) << "\n";
}

static double median(std::vector<double> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

static int cmd_fea() {
    std::ofstream conv("results/fea_convergence.csv");
    conv << "elements,max_midpoint_error_C\n";
    std::cout << "FEA: L=1 m, A=1e-4 m2, k=10 W/(m K), q=10000 W/m3, T(0)=100 C, T(L)=20 C\n";
    for (int n : {10, 20, 40, 80}) {
        FeaResult r = solve_fea(n);
        write_fea_csv(r, "results/fea_N" + std::to_string(n) + ".csv");
        conv << n << "," << r.max_midpoint_error << "\n";
        std::cout << "  N=" << n << "  max midpoint error=" << r.max_midpoint_error << " C\n";
    }
    std::cout << "Debugging insight: the first version imposed BCs by overwriting the "
                 "first/last assembled rows, which double-counted the neighbour stiffness. "
                 "Moving the known terms to the RHS (see fea.hpp) fixed the end nodes.\n";
    return 0;
}

static int cmd_batch(int cases = 200, int mesh = 40) {
    auto batch = generate_batch(cases);
    std::cout << "Batch: cases=" << cases << " mesh=" << mesh << " seed=42\n";
    std::ofstream out("results/performance.csv");
    out << "workers,median_ms,speedup,efficiency\n";
    std::vector<double> serial;
    double t1 = 0;
    for (int workers : {1, 2, 4}) {
        int w = std::min(workers, (int)std::thread::hardware_concurrency());
        if (w < 1) w = 1;
        // Warm-up (excluded), then 5 measured repetitions, median reported.
        if (workers == 1) run_serial(batch, mesh); else run_parallel(batch, mesh, w);
        std::vector<double> times;
        std::vector<double> last;
        for (int rep = 0; rep < 5; ++rep) {
            auto s = std::chrono::steady_clock::now();
            last = (workers == 1) ? run_serial(batch, mesh) : run_parallel(batch, mesh, w);
            auto e = std::chrono::steady_clock::now();
            times.push_back(std::chrono::duration<double, std::milli>(e - s).count());
        }
        double med = median(times);
        if (workers == 1) { serial = last; t1 = med; }
        double speedup = (workers == 1) ? 1.0 : t1 / med;
        double eff = (workers == 1) ? 1.0 : speedup / w;
        out << w << "," << med << "," << speedup << "," << eff << "\n";
        std::cout << "  workers=" << w << " median=" << med << " ms speedup=" << speedup
                  << " efficiency=" << eff << "\n";
        if (workers != 1) {
            double maxdiff = 0;
            for (std::size_t i = 0; i < serial.size(); ++i)
                maxdiff = std::max(maxdiff, std::abs(serial[i] - last[i]));
            std::cout << "  serial/parallel max diff=" << maxdiff << " C (limit 1e-8)\n";
            if (maxdiff > 1e-8) { std::cerr << "FAIL: parallel agreement exceeded 1e-8 C\n"; return 1; }
        }
    }
    std::cout << "  logical CPUs=" << std::thread::hardware_concurrency() << "\n";
    return 0;
}

static int cmd_backprop() {
    BackpropParams p;
    BackpropGrads g = backprop_grads(p);
    std::cout << "Backprop: h=tanh(0.5)=" << std::tanh(0.5) << " y_hat=" << p.v * std::tanh(0.5) + p.c
              << " loss=" << backprop_loss(p) << "\n";
    std::cout << "  dw=" << g.dw << " db=" << g.db << " dv=" << g.dv << " dc=" << g.dc << "\n";
    const double ana[4] = {g.dw, g.db, g.dv, g.dc};
    const char* names[4] = {"w", "b", "v", "c"};
    for (int i = 0; i < 4; ++i) {
        double fd = finite_diff_grad(p, i);
        double err = std::abs(fd - ana[i]);
        std::cout << "  dJ/d" << names[i] << " analytic=" << ana[i] << " finite-diff=" << fd << " err=" << err << "\n";
        if (err > 1e-5) { std::cerr << "FAIL: gradient check exceeded 1e-5\n"; return 1; }
    }
    BackpropParams p1 = backprop_step(p, g, 0.1);
    std::cout << "  after 1 step: w=" << p1.w << " b=" << p1.b << " v=" << p1.v << " c=" << p1.c
              << " loss=" << backprop_loss(p1) << "\n";
    auto traj = loss_trajectory(p, 100, 0.1);
    std::ofstream out("results/backprop_loss.csv");
    out << "step,loss\n";
    for (std::size_t i = 0; i < traj.size(); ++i) out << i << "," << traj[i] << "\n";
    std::cout << "  loss after 100 steps=" << traj.back()
              << " (falls to near zero: 4 parameters can fit this single sample exactly; this is not evidence of generalisation)\n";
    return 0;
}

static int cmd_mock() {
    std::cout << "Devices (SIMULATED mock adapters, no real hardware):\n";
    std::cout << "  [MCU] STM32 Nucleo-style board: USB-CDC host link, 3.3 V domain, SWD/JTAG owned by ST-LINK adapter\n";
    std::cout << "  [FPGA] Artix-7-style board: USB-JTAG host link, 3.3 V I/O with 1.8 V core, JTAG owned by platform cable\n";
    std::cout << "  [JTAG] Debug adapter: host USB, target JTAG (TCK/TMS/TDI/TDO/TRST), level-shifted to target voltage\n";
    std::cout << "  Telemetry: MCU temp=36.5 C (ok) | FPGA done=1 (ok) | JTAG probe: TIMEOUT (recoverable: retry once, then report; no state corrupted)\n";
    std::cout << "  Note: a JTAG adapter is a debug interface, not a generic compute board; it does not run application code.\n";
    return 0;
}

int main(int argc, char** argv) {
    std::string cmd = (argc > 1) ? argv[1] : "all";
    try {
        std::filesystem::create_directories("results");
        if (cmd == "fea") return cmd_fea();
        if (cmd == "batch") return cmd_batch(argc > 2 ? std::stoi(argv[2]) : 200, argc > 3 ? std::stoi(argv[3]) : 40);
        if (cmd == "backprop") return cmd_backprop();
        if (cmd == "mock") return cmd_mock();
        if (cmd == "validate-stl" && argc > 2) {
            auto t = parse_stl_ascii(argv[2]);
            std::cout << "STL OK: " << t.size() << " triangles\n"; return 0;
        }
        if (cmd == "validate-csv" && argc > 2) {
            auto n = parse_ansys_csv(argv[2]);
            std::cout << "CSV OK: " << n.size() << " nodes\n"; return 0;
        }
        if (cmd == "all") {
            int rc = cmd_fea(); if (rc) return rc;
            rc = cmd_batch(); if (rc) return rc;
            rc = cmd_backprop(); if (rc) return rc;
            return cmd_mock();
        }
        std::cerr << "Unknown command: " << cmd << "\n";
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
