// DifferenceScheme.cpp
#include "DifferenceScheme.h"
#include "Quadrature.h"
#include "Progonka.h"

using namespace std;

SchemeCoef buildCoef(const MainTask& p, int n, MethodChoose choose) {
    const double h = 1.0 / n; //Шаг сетки
    SchemeCoef c;

    // Очистка(обнуление) векторов от мусора.
    c.a.assign(n + 1, 0.0);
    c.d.assign(n + 1, 0.0);
    c.fi.assign(n + 1, 0.0);

    // Две вспомогательные функции, возвращающие нам значения 1/k для корректного подсчета 
    // интегралов у наших коэффициентов, где интегралы непосредственно и используются.
    const Func invK1 = [&p](double x) { return 1.0 / p.k1(x); };
    const Func invK2 = [&p](double x) { return 1.0 / p.k2(x); };

    // Узлы считаем как i/n (а не i*h): так точка xi, кратная шагу, попадает в узел точно.
    // И вообще, тут производим рассчет коэффициента a_i.
    for (int i = 1; i <= n; ++i) {
        const double xl = (i - 1) / static_cast<double>(n);
        const double xr = i / static_cast<double>(n);
        c.a[i] = h / integratePiece(invK1, invK2, p.xi, xl, xr, choose);
    }

    //Здесь считаем уже сразу и d_i и fi_i.
    for (int i = 1; i <= n - 1; ++i) {
        const double xl = (2 * i - 1) / (2.0 * n);   // x_{i-0.5}
        const double xr = (2 * i + 1) / (2.0 * n);   // x_{i+0.5}
        c.d[i]   = integratePiece(p.q1, p.q2, p.xi, xl, xr, choose) / h;
        c.fi[i] = integratePiece(p.f1, p.f2, p.xi, xl, xr, choose) / h;
    }

    return c;
}

vector<double> solveScheme(const MainTask& p, int n, MethodChoose choose) {

    const SchemeCoef c = buildCoef(p, n, choose);
    const double h2 = 1.0 / (static_cast<double>(n) * n);   // h^2
    const size_t N = static_cast<size_t>(n) + 1;
    vector<double> sub(N, 0.0), diag(N, 0.0), sup(N, 0.0), rhs(N, 0.0); //Векторы для предстоящего построения матрицы.

    // Граничные условия 1-го рода; заносим сразу их в наши векторы.
    diag[0] = 1.0;  rhs[0] = p.mu1;
    diag[n] = 1.0;  rhs[n] = p.mu2;

    // Внутренние узлы:  -a_i/h^2 v_{i-1} + ((a_i+a_{i+1})/h^2 + d_i) v_i - a_{i+1}/h^2 v_{i+1} = fii_i
    // *домножили на -1, чтобы диагональ сама была положительна.
    for (int i = 1; i <= n - 1; ++i) {
        sub[i]  = -c.a[i] / h2;
        sup[i]  = -c.a[i + 1] / h2;
        diag[i] = (c.a[i] + c.a[i + 1]) / h2 + c.d[i];
        rhs[i]  = c.fi[i];
    }

    return solveTridiagonal(sub, diag, sup, rhs);
}
