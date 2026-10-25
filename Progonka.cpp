// TridiagonalSolver.cpp
#include "Progonka.h"
#include <vector>
#include <stdexcept>

using namespace std;

vector<double> solveTridiagonal(const vector<double>& sub, const vector<double>& diag, const vector<double>& sup, const vector<double>& rhs) {
    const size_t N = diag.size();

    if (N == 0) {
        throw invalid_argument("Пустая система отправлена в solveTridiagonal.");
    }
    if (sub.size() != N || sup.size() != N || diag.size() != N) {
        throw invalid_argument("Массивы диагоналей содержат разное количество элементов! Некорректно!");
    }


    vector<double> alpha(N), beta(N), y(N);

    // Прямой ход прогонки: y[i] = alpha[i+1]*y[i+1] + beta[i+1]
    alpha[0] = -sup[0] / diag[0]; //Здесь начальные значение в нуле, так как массив с нуля начинается.
    beta[0]  =  rhs[0] / diag[0];
    for (size_t i = 1; i < N; ++i) {
        const double denom = diag[i] + sub[i] * alpha[i - 1];
        alpha[i] = -sup[i] / denom;                       // для i=N-1 sup=0 -> alpha=0
        beta[i]  = (rhs[i] - sub[i] * beta[i - 1]) / denom;
    }

    // Обратный ход
    y[N - 1] = beta[N - 1];
    for (size_t i = N - 1; i-- > 0;)
        y[i] = alpha[i] * y[i + 1] + beta[i];

    return y;
}
