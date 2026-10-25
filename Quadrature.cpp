// Quadrature.cpp
#include "Quadrature.h"
#include <cmath>

using namespace std;

// Здесь мы производим вычисления наших интегралов, необходимых для отыскания коэффициентов 
// на метод баланса; то есть, просто алгоритм для подсчета интегралов универсальный.

namespace {
// Формула Гаусса-Лежандра с 5 узлами на [a,b]
// Идея: входящий интеграл заменяется на взвешенную сумму значений функции в нескольких
// специально подобранных точках. Значения для X[] и W[] лучше взять уже вычисленные, они 
// всегда одинаковы; https://en.wikipedia.org/wiki/Gaussian_quadrature
double gauss5(const Func& f, double a, double b) {
    static const double X[3] = {0.5384693101056831, 0.9061798459386640, 0.0};
    static const double W[3] = {0.4786286704993665, 0.2369268850561891, 0.5688888888888889};
    const double c = 0.5 * (a + b), h = 0.5 * (b - a);
    double s = W[2] * f(c);
    for (int i = 0; i < 2; ++i)
        s += W[i] * (f(c - h * X[i]) + f(c + h * X[i]));

    return s * h;
}

// Адаптивное деление: сравниваем "весь отрезок" и "две половины"
double adaptive(const Func& f, double a, double b, double whole, double tol, int depth) { //где: whole - сам полный отрезок, tol - переменная для точности, depth - ограничение глубины рекурсии(при ограничении на 40, точность примерно до 10^(-12)
    const double m = 0.5 * (a + b);
    const double l = gauss5(f, a, m); //l - левая половина разделения
    const double r = gauss5(f, m, b); //r - правая половина разделения

    if (depth >= 40 || fabs(l + r - whole) <= tol) {
        return l + r;
    }

    return adaptive(f, a, m, l, tol, depth + 1) + adaptive(f, m, b, r, tol, depth + 1); //для продолжения вычислений по рекурсии
}
} // namespace



double integrate(const Func& f, double a, double b) { //точка входа для вычисления 
    if (b <= a) {
        return 0.0;
    }

    const double whole = gauss5(f, a, b);
    const double tol = 1e-14 * fabs(whole) + 1e-300;
    return adaptive(f, a, b, whole, tol, 0);
}

double Midpoint(const Func& f, double a, double b) {
    if (b <= a) {
        return 0.0;
    }

    return (b - a) * f(0.5 * (a + b));
}

double integratePiece(const Func& f1, const Func& f2, double xi, double a, double b, MethodChoose choose) {

    /*auto integ = integrate;
    if (choose == MethodChoose::Midpoint) {
        integ = Midpoint;
    }*/

    const auto integ = (choose == MethodChoose::Midpoint) ? Midpoint : integrate;

    if (b <= xi) return integ(f1, a, b);      // целиком слева от разрыва
    if (a >= xi) return integ(f2, a, b);      // целиком справа
    return integ(f1, a, xi) + integ(f2, xi, b);  // разрыв внутри
}
