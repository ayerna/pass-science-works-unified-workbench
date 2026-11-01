#pragma once
// Handwritten-style backprop, implemented without autodiff.
// x=1, y=2, h=tanh(w*x+b), y_hat=v*h+c, J=0.5*(y_hat-y)^2.
// Start: w=0.5, b=0, v=1, c=0, lr=0.1. One simultaneous update.
#include <cmath>
#include <vector>

namespace wb {

struct BackpropParams { double w = 0.5, b = 0.0, v = 1.0, c = 0.0; };
struct BackpropGrads { double dw = 0, db = 0, dv = 0, dc = 0; };

inline double backprop_loss(const BackpropParams& p, double x = 1.0, double y = 2.0) {
    const double h = std::tanh(p.w * x + p.b);
    const double yhat = p.v * h + p.c;
    return 0.5 * (yhat - y) * (yhat - y);
}

inline BackpropGrads backprop_grads(const BackpropParams& p, double x = 1.0, double y = 2.0) {
    const double z = p.w * x + p.b;
    const double h = std::tanh(z);
    const double yhat = p.v * h + p.c;
    const double dJ_dyhat = yhat - y;          // dJ/dy_hat
    BackpropGrads g;
    g.dv = dJ_dyhat * h;                      // dy_hat/dv = h
    g.dc = dJ_dyhat;                          // dy_hat/dc = 1
    const double dJ_dh = dJ_dyhat * p.v;      // dy_hat/dh = v
    const double dh_dz = 1.0 - h * h;         // d tanh(z)/dz = sech^2(z)
    const double dJ_dz = dJ_dh * dh_dz;
    g.dw = dJ_dz * x;                         // dz/dw = x
    g.db = dJ_dz;                             // dz/db = 1
    return g;
}

inline BackpropParams backprop_step(const BackpropParams& p, const BackpropGrads& g, double lr = 0.1) {
    // Simultaneous update: every gradient comes from the ORIGINAL parameters.
    return {p.w - lr * g.dw, p.b - lr * g.db, p.v - lr * g.dv, p.c - lr * g.dc};
}

// Central finite difference for one parameter, by index 0=w,1=b,2=v,3=c.
inline double finite_diff_grad(const BackpropParams& p, int idx, double eps = 1e-6) {
    BackpropParams plus = p, minus = p;
    double* pp = (idx == 0 ? &plus.w : idx == 1 ? &plus.b : idx == 2 ? &plus.v : &plus.c);
    double* pm = (idx == 0 ? &minus.w : idx == 1 ? &minus.b : idx == 2 ? &minus.v : &minus.c);
    *pp += eps; *pm -= eps;
    return (backprop_loss(plus) - backprop_loss(minus)) / (2.0 * eps);
}

inline std::vector<double> loss_trajectory(BackpropParams p, int steps = 100, double lr = 0.1) {
    std::vector<double> out;
    out.push_back(backprop_loss(p));
    for (int i = 0; i < steps; ++i) {
        p = backprop_step(p, backprop_grads(p), lr);
        out.push_back(backprop_loss(p));
    }
    return out;
}

}  // namespace wb
