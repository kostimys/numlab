#include "Problem.h"

MainTask makeMainProblem() {
    MainTask p;
    // ТВОЙ ВАРИАНТ 2 (с добавкой +0.1 для k1)
    p.k1 = [](double x) { return sqrt(x) + 0.1; };
    p.k2 = [](double x) { return x + 1.0; };
    p.q1 = [](double x) { return 1.0; };
    p.q2 = [](double x) { return x * x; };
    p.f1 = [](double x) { return 1.0; };
    p.f2 = [](double x) { return 2.0 + sqrt(x); };
    p.xi = 0.25;
    p.mu1 = 0.0;
    p.mu2 = 1.0; // не используется для смешанной
    
    // Параметры для смешанной задачи (ВПИШИ СВОИ, если другие)
    p.alpha = 5.0; 
    p.u_cp = 10.0; 
    return p;
}

TestTask makeTestProblem(const MainTask& m) {
    TestTask t;
    t.k1 = m.k1(m.xi - 1e-9); 
    t.k2 = m.k2(m.xi + 1e-9); 
    t.q1 = m.q1(m.xi - 1e-9);
    t.q2 = m.q2(m.xi + 1e-9);
    t.f1 = m.f1(m.xi - 1e-9);
    t.f2 = m.f2(m.xi + 1e-9);
    t.xi = m.xi; 
    t.mu1 = m.mu1; 
    t.mu2 = m.mu2;
    t.bc = m.bc;
    t.alpha = m.alpha;
    t.u_cp = m.u_cp;
    return t;
}

MainTask TestTask::toProblem() const {
    MainTask p;
    double K1=k1, K2=k2, Q1=q1, Q2=q2, F1=f1, F2=f2;
    p.k1 = [K1](double){ return K1; }; p.k2 = [K2](double){ return K2; };
    p.q1 = [Q1](double){ return Q1; }; p.q2 = [Q2](double){ return Q2; };
    p.f1 = [F1](double){ return F1; }; p.f2 = [F2](double){ return F2; };
    p.xi=xi; p.mu1=mu1; p.mu2=mu2;
    p.bc=bc; p.alpha=alpha; p.u_cp=u_cp;
    return p;
}