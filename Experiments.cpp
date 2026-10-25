// Experiments.cpp
#include "Experiments.h"
#include "AnalyticSolution.h"
#include "DifferenceScheme.h"
#include <algorithm>
#include <cmath>

using namespace std;

TestResult runTestProblem(const TestTask& t, int n, MethodChoose choose) {
    TestResult r;
    r.n = n;
    r.choose = choose;
    const MainTask p = t.toProblem();
    const AnalyticSolution exact(t);
    r.v = solveScheme(p, n, choose);

    r.x.resize(n + 1);  
    r.u.resize(n + 1);  
    r.diff.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
        r.x[i] = i / static_cast<double>(n);
        r.u[i] = exact(r.x[i]);
        r.diff[i] = fabs(r.u[i] - r.v[i]);
        if (r.diff[i] > r.eps1) { 
            r.eps1 = r.diff[i]; 
            r.idMax = i; 
        }
    }

    r.Max = r.x[r.idMax];
    return r;
}


MainResult runMainProblem(const MainTask& p, int n, MethodChoose choose) {
    MainResult r;
    r.n = n;
    r.choose = choose;
    r.v  = solveScheme(p, n, choose);
    r.v2 = solveScheme(p, 2 * n, choose); //С шагом(количеством узлов) сетки вдвое меньше тут получается.

    r.x.resize(n + 1);  
    r.diff.resize(n + 1);
    vector<double> v2common(n + 1); //Вектор общих узлов.
    for (int i = 0; i <= n; ++i) {
        r.x[i] = i / static_cast<double>(n);
        v2common[i] = r.v2[2 * i];                  // общие узлы: x_i = x2_{2i}
        r.diff[i] = fabs(r.v[i] - v2common[i]);
        if (r.diff[i] > r.eps2) { 
            r.eps2 = r.diff[i]; 
            r.idMax = i; 
        }
    }

    r.v2 = move(v2common);   // сохраняем v2 только в общих узлах
    r.Max = r.x[r.idMax];
    return r;
}

namespace {
template <class ErrFn>
int findN(ErrFn err, double eps, int nStart, int nMax) {
    int lo = 0, hi = nStart;
    // удвоение, пока точность не достигнута
    while (err(hi) > eps) {
        lo = hi;
        hi *= 2;
        if (hi > nMax) return 0;
    }

    // бисекция(деление пополам) между lo (не подходит) и hi (подходит)
    while (hi - lo > 1) {
        const int mid = (lo + hi) / 2;
        if (err(mid) <= eps) hi = mid; 
        else lo = mid;
    }
    return hi;
}
} // namespace



int findNForTest(const TestTask& t, MethodChoose choose, double eps, int nStart, int nMax) {
    return findN([&](int n) { return runTestProblem(t, n, choose).eps1; }, eps, nStart, nMax);
}
int findNForMain(const MainTask& p, MethodChoose choose, double eps, int nStart, int nMax) {
    return findN([&](int n) { return runMainProblem(p, n, choose).eps2; }, eps, nStart, nMax);
}



vector<ErrorRow> collectErrors(const TestTask& t, const MainTask& p, const vector<int>& ns, bool doTest, bool doMain, MethodChoose choose) {
    vector<ErrorRow> rows;

    for (size_t i = 0; i < ns.size(); ++i) {
        int n = ns[i];

        ErrorRow row{n, 0.0, 0.0};
        if (doTest) { row.eps1 = runTestProblem(t, n, choose).eps1; }
        if (doMain) { row.eps2 = runMainProblem(p, n, choose).eps2; }
        rows.push_back(row);
    }

    return rows;
}
