// Problem.cpp
#include "Problem.h"
#include <cmath>
#include <iostream>

using namespace std;

// Здесь по-сути все еще нет какой-либо реализации самих методов, однако,
// отсюда вносим все заданные НУ и функции для дальнейшего вычисления значений.

// Берем и переводим числа в функции по-сути данным действием.
MainTask TestTask::toProblem() const {
    MainTask p;
    const double K1 = k1, K2 = k2, Q1 = q1, Q2 = q2, F1 = f1, F2 = f2;
    p.k1 = [K1](double) { return K1; };
    p.k2 = [K2](double) { return K2; };
    p.q1 = [Q1](double) { return Q1; };
    p.q2 = [Q2](double) { return Q2; };
    p.f1 = [F1](double) { return F1; };
    p.f2 = [F2](double) { return F2; };
    p.xi = xi;  
    p.mu1 = mu1;  
    p.mu2 = mu2;
    return p;
}

// ----------------------------------------------------------------------------
// ВАРИАНТ 2 (Таблица 1):
//   xi = 0.25, mu1 = 0, mu2 = 1
//   k1 = sqrt(x) + 0.1 ,  k2 = x + 1
//   q1 = 1             ,  q2 = x^2
//   f1 = 1             ,  f2 = 2 + sqrt(x)
// (к sqrt(x) в k1 добавлено 0.1, чтобы k(x) > 0 на всем отрезке)
// Если в таблице корень стоит в других коэффициентах - менять нужно только здесь.
// ----------------------------------------------------------------------------
MainTask makeMainProblem() {
    MainTask p;
    p.k1 = [](double x) { return sqrt(x) + 0.1; };
    p.k2 = [](double x) { return x + 1.0; };
    p.q1 = [](double x)   { return 1.0; };
    p.q2 = [](double x) { return x * x; };
    p.f1 = [](double x)   { return 1.0; };
    p.f2 = [](double x) { return 2.0 + sqrt(x); };
    p.xi  = 0.25;
    p.mu1 = 0.0;
    p.mu2 = 1.0;
    return p;
}

TestTask makeTestProblem(const MainTask& m) {
    TestTask t;
    t.k1 = m.k1(m.xi);  
    t.k2 = m.k2(m.xi);   // lim x->xi-0  и  lim x->xi+0
    t.q1 = m.q1(m.xi);  
    t.q2 = m.q2(m.xi);
    t.f1 = m.f1(m.xi);  
    t.f2 = m.f2(m.xi);
    t.xi = m.xi;  
    t.mu1 = m.mu1;  
    t.mu2 = m.mu2;
    return t;
}
