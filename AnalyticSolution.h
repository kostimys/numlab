#pragma once
#include "Problem.h"

class AnalyticSolution {
public:
    explicit AnalyticSolution(const TestTask& t);
    double operator()(double x) const;

private:
    struct Part {
        double k, q, f, lambda;
        bool hasQ;
        double basis(int j, double s) const;
        double dbasis(int j, double s) const;
        double part(double s) const;
        double dpart(double s) const;
    };
    Part L_, R_;
    double xi_;
    double c_[4];
};