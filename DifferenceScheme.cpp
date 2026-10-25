#include "DifferenceScheme.h"
#include "Quadrature.h"
#include "Progonka.h"

std::vector<double> solveScheme(const MainTask& p, int n) {
    const double h = 1.0 / n; 
    const double h2 = h * h;
    std::vector<double> a(n + 1, 0.0), d(n + 1, 0.0), fi(n + 1, 0.0);

    const Func invK1 = [&p](double x) { return 1.0 / p.k1(x); };
    const Func invK2 = [&p](double x) { return 1.0 / p.k2(x); };

    for (int i = 1; i <= n; ++i) {
        const double xl = (i - 1) / static_cast<double>(n);
        const double xr = i / static_cast<double>(n);
        a[i] = h / integratePiece(invK1, invK2, p.xi, xl, xr);
    }

    for (int i = 1; i <= n - 1; ++i) {
        const double xl = (2 * i - 1) / (2.0 * n);
        const double xr = (2 * i + 1) / (2.0 * n);
        d[i]   = integratePiece(p.q1, p.q2, p.xi, xl, xr) / h;
        fi[i]  = integratePiece(p.f1, p.f2, p.xi, xl, xr) / h;
    }

    const size_t N = static_cast<size_t>(n) + 1;
    std::vector<double> sub(N, 0.0), diag(N, 0.0), sup(N, 0.0), rhs(N, 0.0);

    // Левый край: u(0) = mu1
    diag[0] = 1.0;  rhs[0] = p.mu1;

    // Внутренние узлы
    for (int i = 1; i <= n - 1; ++i) {
        sub[i]  = -a[i] / h2;
        sup[i]  = -a[i + 1] / h2;
        diag[i] = (a[i] + a[i + 1]) / h2 + d[i];
        rhs[i]  = fi[i];
    }

    // Правый край (в зависимости от BCType)
    if (p.bc == BCType::Dirichlet) {
        diag[n] = 1.0;  rhs[n] = p.mu2;
    } 
    else if (p.bc == BCType::MixedClassic) {
        double kn = p.k2(1.0);
        sub[n]  = -kn / h;
        diag[n] = kn / h + p.alpha;
        rhs[n]  = p.alpha * p.u_cp;
    } 
    else if (p.bc == BCType::MixedImproved) {
        double kn = p.k2(1.0);
        double xl = 1.0 - h/2.0;
        double d_n = integratePiece(p.q1, p.q2, p.xi, xl, 1.0) / (h/2.0);
        double f_n = integratePiece(p.f1, p.f2, p.xi, xl, 1.0) / (h/2.0);
        
        sub[n]  = -a[n] / h2;
        diag[n] = a[n] / h2 + (d_n / 2.0) + (p.alpha / h);
        rhs[n]  = (f_n / 2.0) + (p.alpha * p.u_cp / h);
    }

    return solveTridiagonal(sub, diag, sup, rhs);
}