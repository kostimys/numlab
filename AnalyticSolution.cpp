// AnalyticSolution.cpp
#include "AnalyticSolution.h"
#include <cmath>
#include <utility>
#include <stdexcept>

// Обработка входящих коэффициентов и дальнейшее вычисление коэффициентов матрицы.
// Также вычисление матрицы 4х5 методом Гаусса, и вычисление констант для общего решения нашей тестовой задачи; есть небольшое объяснение от дипсика.

using namespace std;

double AnalyticSolution::Part::basis(int j, double s) const {
    if (hasQ) {
        if (j == 0) {
            return cosh(lambda * s);
        }
        else {
            return sinh(lambda * s);
        }
    } 
    else {
        if (j == 0) {
            return 1.0;
        }
        else {
            return s;
        }
    }
}

double AnalyticSolution::Part::dbasis(int j, double s) const {
    if (hasQ) {
        if (j == 0) {
            return lambda * sinh(lambda * s);
        }
        else {
            return lambda * cosh(lambda * s);
        }
    }
    else {
        if (j == 0) {
            return 0.0;
        }
        else {
            return 1.0;
        }
    }
}

double AnalyticSolution::Part::part(double s) const {
    if (hasQ) {
        return (f / q);
    }
    else {
        return (2.0 * k) * s * s;
    }
}

double AnalyticSolution::Part::dpart(double s) const {
    if (hasQ) {
        return 0.0;
    }
    else {
        return (-f / (k * s));
    }
}



namespace {
// Решение СЛАУ 4x4 обычным методом Гаусса с выбором главного элемента
void gauss4(double A[4][5], double x[4]) {
    for (int c = 0; c < 4; ++c) {
        int p = c;
        for (int r = c + 1; r < 4; ++r) {
            if (fabs(A[r][c]) > fabs(A[p][c])) { p = r; }
        }
        if (fabs(A[p][c]) < 1e-300) { throw runtime_error("Вырожденная матрица в аналитическом решении! Ошибка!"); }
        
        for (int j = 0; j < 5; ++j) { swap(A[c][j], A[p][j]); }
        for (int r = c + 1; r < 4; ++r) {
            double m = A[r][c] / A[c][c];
            for (int j = c; j < 5; ++j) { A[r][j] -= m * A[c][j]; }
        }
    }

    for (int r = 3; r >= 0; --r) {
        double s = A[r][4];
        for (int j = r + 1; j < 4; ++j) { s -= A[r][j] * x[j]; }
        x[r] = s / A[r][r];
    }
}
} // namespace

AnalyticSolution::AnalyticSolution(const TestTask& t) : xi_(t.xi) {
    L_ = {t.k1, t.q1, t.f1, 0.0, t.q1 > 1e-14};
    R_ = {t.k2, t.q2, t.f2, 0.0, t.q2 > 1e-14};
    if (L_.hasQ) { L_.lambda = sqrt(L_.q / L_.k); }
    if (R_.hasQ) { R_.lambda = sqrt(R_.q / R_.k); }

    double A[4][5] = {};
    // u(0) = mu1  (левый участок, s = -xi)
    A[0][0] = L_.basis(0, -xi_);  A[0][1] = L_.basis(1, -xi_);
    A[0][4] = t.mu1 - L_.part(-xi_);
    // u(1) = mu2  (правый участок, s = 1-xi)
    A[1][2] = R_.basis(0, 1.0 - xi_);  A[1][3] = R_.basis(1, 1.0 - xi_);
    A[1][4] = t.mu2 - R_.part(1.0 - xi_);
    // непрерывность температуры в xi (s = 0)
    A[2][0] = L_.basis(0, 0);  A[2][1] = L_.basis(1, 0);
    A[2][2] = -R_.basis(0, 0); A[2][3] = -R_.basis(1, 0);
    A[2][4] = R_.part(0) - L_.part(0);
    // непрерывность теплового потока: k1 u'(xi-0) = k2 u'(xi+0)
    A[3][0] = L_.k * L_.dbasis(0, 0);  A[3][1] = L_.k * L_.dbasis(1, 0);
    A[3][2] = -R_.k * R_.dbasis(0, 0); A[3][3] = -R_.k * R_.dbasis(1, 0);
    A[3][4] = R_.k * R_.dpart(0) - L_.k * L_.dpart(0);

    gauss4(A, c_);
}

double AnalyticSolution::operator()(double x) const {
    const double s = x - xi_;
    if (x <= xi_) {
        return c_[0] * L_.basis(0, s) + c_[1] * L_.basis(1, s) + L_.part(s);
    }

    return c_[2] * R_.basis(0, s) + c_[3] * R_.basis(1, s) + R_.part(s);
}
