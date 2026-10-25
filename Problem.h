#pragma once
#include <functional>
#include <cmath>

using Func = std::function<double(double)>;

enum class BCType {
    Dirichlet,          // 1-2 чел: u(1) = mu2
    MixedClassic,       // 3 чел: классическая аппроксимация
    MixedImproved       // 4 чел: улучшенная аппроксимация
};

struct MainTask {
    Func k1, k2, q1, q2, f1, f2;
    double xi  = 0.5;
    double mu1 = 0.0;
    double mu2 = 0.0;
    
    BCType bc = BCType::Dirichlet; 
    double alpha = 5.0; 
    double u_cp = 10.0;
};

struct TestTask {
    double k1, k2, q1, q2, f1, f2;
    double xi, mu1, mu2;
    
    BCType bc = BCType::Dirichlet;
    double alpha = 5.0;
    double u_cp = 10.0;

    MainTask toProblem() const;
};

MainTask makeMainProblem();
TestTask makeTestProblem(const MainTask& m);