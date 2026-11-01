#pragma once
// 1D steady thermal FEA, uniform rod, two-node linear elements.
// Equation: -d/dx(k*A*dT/dx) = q*A, constant k, A, q, insulated sides.
// BCs: T(0)=100 C, T(L)=20 C. L=1 m, A=1e-4 m^2 by default in main.
#include <cmath>
#include <stdexcept>
#include <vector>

namespace wb {

struct FeaResult {
    int n = 0;
    std::vector<double> x;      // nodes, size n+1
    std::vector<double> t;      // nodal temperatures, size n+1
    double max_midpoint_error = 0.0;
};

inline double exact_temperature(double x, double L, double k, double q) {
    // T_exact = 100 - 80*x/L + (q/(2k))*x*(L-x), for the brief's BCs.
    // This closed form is only exact for T(0)=100, T(L)=20.
    return 100.0 - 80.0 * x / L + (q / (2.0 * k)) * x * (L - x);
}

// Thomas algorithm for a tridiagonal system. a: sub, b: diag, c: super, d: rhs.
inline std::vector<double> solve_tridiagonal(std::vector<double> a,
                                             std::vector<double> b,
                                             std::vector<double> c,
                                             std::vector<double> d) {
    const std::size_t m = d.size();
    if (m == 0) throw std::invalid_argument("empty tridiagonal system");
    for (std::size_t i = 1; i < m; ++i) {
        if (b[i - 1] == 0.0) throw std::runtime_error("zero pivot in FEA solve");
        const double w = a[i] / b[i - 1];
        b[i] -= w * c[i - 1];
        d[i] -= w * d[i - 1];
    }
    std::vector<double> out(m, 0.0);
    out[m - 1] = d[m - 1] / b[m - 1];
    for (std::size_t ii = m - 1; ii-- > 0;) {
        out[ii] = (d[ii] - c[ii] * out[ii + 1]) / b[ii];
    }
    return out;
}

inline FeaResult solve_fea(int n, double L = 1.0, double A = 1e-4,
                           double k = 10.0, double q = 10000.0,
                           double t0 = 100.0, double tL = 20.0) {
    if (n < 1) throw std::invalid_argument("element count must be >= 1");
    if (!(L > 0 && A > 0 && k > 0)) throw std::invalid_argument("L, A, k must be positive");
    const double h = L / n;
    // Element stiffness ke = k*A/h * [[1,-1],[-1,1]]; element load fe = q*A*h/2 * [1,1].
    const double ke = k * A / h;
    const double fe = q * A * h / 2.0;
    // Unknowns are interior nodes 1..n-1. Assemble tridiagonal system.
    const int m = n - 1;
    std::vector<double> a(m, 0.0), b(m, 2.0 * ke), c(m, 0.0), d(m, 2.0 * fe);
    for (int i = 0; i < m; ++i) {
        if (i > 0) a[i] = -ke;
        if (i < m - 1) c[i] = -ke;
    }
    // Impose Dirichlet BCs correctly: move known terms to the RHS.
    if (m >= 1) {
        d[0] += ke * t0;
        d[m - 1] += ke * tL;
    }
    std::vector<double> interior = (m > 0) ? solve_tridiagonal(a, b, c, d) : std::vector<double>{};
    FeaResult r;
    r.n = n;
    r.x.resize(n + 1);
    r.t.resize(n + 1);
    for (int i = 0; i <= n; ++i) r.x[i] = i * h;
    r.t[0] = t0;
    r.t[n] = tL;
    for (int i = 1; i < n; ++i) r.t[i] = interior[i - 1];
    // Midpoint error uses FE linear interpolation: (T_i + T_{i+1}) / 2.
    double max_err = 0.0;
    for (int e = 0; e < n; ++e) {
        const double xm = 0.5 * (r.x[e] + r.x[e + 1]);
        const double fe_mid = 0.5 * (r.t[e] + r.t[e + 1]);
        const double err = std::abs(fe_mid - exact_temperature(xm, L, k, q));
        if (err > max_err) max_err = err;
    }
    r.max_midpoint_error = max_err;
    return r;
}

}  // namespace wb
