// Experiments.h
// Решение тестовой и основной задач, подбор сетки, таблицы порядков.
#pragma once
#include "Problem.h"
#include "Quadrature.h"
#include <vector>

using namespace std;

constexpr double kEps = 0.5e-6;   // заданная точность



// --- Тестовая задача: сравнение с аналитическим решением ---
struct TestResult {
    int n = 0;
    MethodChoose choose = MethodChoose::Gauss;
    vector<double> x, u, v, diff;        // diff[i] = |u(x_i) - v(x_i)|
    double eps1 = 0.0;                   // max |u - v|
    int idMax = 0;                       // номер узла, где достигается максимум
    double Max = 0.0;
};

// --- Основная задача: сравнение решений на сетках n и 2n ---
struct MainResult {
    int n = 0;
    MethodChoose choose = MethodChoose::Gauss;
    vector<double> x, v, v2, diff;       // v2[i] = v2(x_{2i}), diff[i] = |v - v2|
    double eps2 = 0.0;                   // max |v(x_i) - v2(x_2i)|
    int idMax = 0;                       // номер узла, где достигается максимум
    double Max = 0.0;
};

TestResult runTestProblem(const TestTask& t, int n, MethodChoose choose = MethodChoose::Gauss);
MainResult runMainProblem(const MainTask& p, int n, MethodChoose choose = MethodChoose::Gauss);


// Подбор такое n (удвоение + бисекция), при котором eps1 <= eps / eps2 <= eps.
// Возвращает 0, если за nMax не достигнуто.
int findNForTest(const TestTask& t, MethodChoose choose = MethodChoose::Gauss, double eps = kEps, int nStart = 10, int nMax = 4000000);
int findNForMain(const MainTask& p, MethodChoose choose = MethodChoose::Gauss, double eps = kEps, int nStart = 10, int nMax = 2000000);


// Строка таблиц 4/5/6
struct ErrorRow {
    int n;
    double eps1;   // тестовая задача
    double eps2;   // основная задача
};

vector<ErrorRow> collectErrors(const TestTask& t, const MainTask& p, const vector<int>& ns, bool doTest = true, bool doMain = true, MethodChoose choose = MethodChoose::Gauss);
