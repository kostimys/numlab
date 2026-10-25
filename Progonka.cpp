#include "Progonka.h"
#include <stdexcept>

std::vector<double> solveTridiagonal(const std::vector<double>& sub, const std::vector<double>& diag, const std::vector<double>& sup, const std::vector<double>& rhs) {
    const size_t N = diag.size();
    if (N == 0) throw std::invalid_argument("Empty system");

    std::vector<double> alpha(N), beta(N), y(N);
    alpha[0] = -sup[0] / diag[0];
    beta[0]  =  rhs[0] / diag[0];
    for (size_t i = 1; i < N; ++i) {
        const double denom = diag[i] + sub[i] * alpha[i - 1];
        alpha[i] = -sup[i] / denom;
        beta[i]  = (rhs[i] - sub[i] * beta[i - 1]) / denom;
    }
    y[N - 1] = beta[N - 1];
    for (size_t i = N - 1; i-- > 0;)
        y[i] = alpha[i] * y[i + 1] + beta[i];
    return y;
}