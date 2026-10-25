// AnalyticSolution.h
// Аналитическое решение тестовой задачи (кусочно-постоянные коэффициенты).
//
// На каждом из участков k u'' - q u = -f. Общее решение:
//   q > 0:  u = C1 ch(l(x-xi)) + C2 sh(l(x-xi)) + f/q,   l = sqrt(q/k)
//   q = 0:  u = C1 + C2 (x-xi) - f/(2k) (x-xi)^2
// Четыре константы находятся из: u(0)=mu1, u(1)=mu2,
// условий сопряжения в xi: u(xi-0)=u(xi+0), k1 u'(xi-0)=k2 u'(xi+0).
#pragma once
#include "Problem.h"

using namespace std;

class AnalyticSolution {
public:
    explicit AnalyticSolution(const TestTask& t);
    double operator()(double x) const;

private:
    struct Part {
        double k, q, f, lambda;
        bool hasQ;
        double basis(int j, double s) const;    // базисные функции, s = x - xi
        double dbasis(int j, double s) const;   // их производные
        double part(double s) const;            // частное решение
        double dpart(double s) const;           // его производная
    };

    Part L_, R_;
    double xi_;
    double c_[4];   // C1,C2 (слева), C1,C2 (справа)
};
