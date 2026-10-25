#include "Quadrature.h"
#include <cmath>

namespace {
    double gauss5(const Func& f, double a, double b) {
        static const double X[3] = {0.5384693101056831, 0.9061798459386640, 0.0};
        static const double W[3] = {0.4786286704993665, 0.2369268850561891, 0.5688888888888889};
        const double c = 0.5 * (a + b), h = 0.5 * (b - a);
        double s = W[2] * f(c);
        for (int i = 0; i < 2; ++i)
            s += W[i] * (f(c - h * X[i]) + f(c + h * X[i]));
        return s * h;
    }

    double adaptive(const Func& f, double a, double b, double whole, double tol, int depth) {
        const double m = 0.5 * (a + b);
        const double l = gauss5(f, a, m);
        const double r = gauss5(f, m, b);
        if (depth >= 40 || fabs(l + r - whole) <= tol) return l + r;
        return adaptive(f, a, m, l, tol, depth + 1) + adaptive(f, m, b, r, tol, depth + 1);
    }
}

double integrate(const Func& f, double a, double b) {
    if (b <= a) return 0.0;
    const double whole = gauss5(f, a, b);
    const double tol = 1e-14 * fabs(whole) + 1e-300;
    return adaptive(f, a, b, whole, tol, 0);
}

double Midpoint(const Func& f, double a, double b) {
    if (b <= a) return 0.0;
    return (b - a) * f(0.5 * (a + b));
}

double integratePiece(const Func& f1, const Func& f2, double xi, double a, double b, MethodChoose choose) {
    const auto integ = (choose == MethodChoose::Midpoint) ? Midpoint : integrate;
    if (b <= xi) return integ(f1, a, b);
    if (a >= xi) return integ(f2, a, b);
    return integ(f1, a, xi) + integ(f2, xi, b);
}