#include "Problem.h"
#include "DifferenceScheme.h"
#include "AnalyticSolution.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <filesystem>

using namespace std;

int main(int argc, char* argv[]) {
    int n = 40;
    string bc_str = "TestMixed";
    double target_eps = 0.5e-6;
    bool auto_mode = false;

    // Если передан флаг -auto, включаем автоподбор
    if (argc >= 2 && string(argv[1]) == "-auto") {
        auto_mode = true;
        if (argc >= 3) bc_str = argv[2];
        if (argc >= 4) target_eps = stod(argv[3]);
    } else {
        if (argc >= 2) n = stoi(argv[1]);
        if (argc >= 3) bc_str = argv[2];
    }

    MainTask p = makeMainProblem();
    TestTask t = makeTestProblem(p);
    filesystem::create_directories("results");
    ofstream f("results/result.csv");

    // Лямбда для расчета ошибки при заданном n
    auto calculate_error = [&](int current_n) -> double {
        if (bc_str == "TestDirichlet") {
            p.bc = BCType::Dirichlet; t.bc = BCType::Dirichlet;
            auto tp = t.toProblem(); auto v = solveScheme(tp, current_n);
            AnalyticSolution exact(t);
            double eps = 0;
            for (int i = 0; i <= current_n; ++i) {
                double x = i / (double)current_n;
                eps = max(eps, abs(exact(x) - v[i]));
            }
            return eps;
        } else if (bc_str == "TestMixed") {
            p.bc = BCType::MixedClassic; t.bc = BCType::MixedClassic;
            auto tp = t.toProblem(); auto v = solveScheme(tp, current_n);
            AnalyticSolution exact(t);
            double eps = 0;
            for (int i = 0; i <= current_n; ++i) {
                double x = i / (double)current_n;
                eps = max(eps, abs(exact(x) - v[i]));
            }
            return eps;
        } else if (bc_str == "MainDirichlet") {
            p.bc = BCType::Dirichlet;
            auto v = solveScheme(p, current_n); auto v2 = solveScheme(p, 2 * current_n);
            double eps = 0;
            for (int i = 0; i <= current_n; ++i) eps = max(eps, abs(v[i] - v2[2 * i]));
            return eps;
        } else { // MainMixed
            p.bc = BCType::MixedImproved;
            auto v = solveScheme(p, current_n); auto v2 = solveScheme(p, 2 * current_n);
            double eps = 0;
            for (int i = 0; i <= current_n; ++i) eps = max(eps, abs(v[i] - v2[2 * i]));
            return eps;
        }
    };

    // Автоподбор сетки
    if (auto_mode) {
        n = 10;
        while (calculate_error(n) > target_eps && n < 100000) {
            n *= 2;
        }
        cout << "[АВТОПОДБОР] Для достижения eps = " << target_eps << " выбрано n = " << n << "\n";
    }

    // Основные расчеты и сохранение в файл
    if (bc_str == "TestDirichlet" || bc_str == "TestMixed") {
        if (bc_str == "TestDirichlet") { p.bc = BCType::Dirichlet; t.bc = BCType::Dirichlet; }
        else { p.bc = BCType::MixedClassic; t.bc = BCType::MixedClassic; }

        auto tp = t.toProblem(); auto v = solveScheme(tp, n);
        AnalyticSolution exact(t);

        f << "Узел;x;u(x);v(x);Разность\n";
        double eps1 = 0; double max_x = 0;
        for (int i = 0; i <= n; ++i) {
            double x = i / (double)n;
            double u = exact(x);
            double diff = abs(u - v[i]);
            if (diff > eps1) { eps1 = diff; max_x = x; }
            f << i << ";" << x << ";" << u << ";" << v[i] << ";" << diff << "\n";
        }
        cout << "=== " << (bc_str == "TestDirichlet" ? "ТЕСТОВАЯ (1-я краевая)" : "ТЕСТОВАЯ (Смешанная)") << " ===\n";
        cout << "n = " << n << "\n";
        cout << "eps1 = " << scientific << setprecision(6) << eps1 << " (задано: <= " << target_eps << ")\n";
        cout << "Макс. отклонение в x = " << fixed << setprecision(4) << max_x << "\n";
    }
    else {
        if (bc_str == "MainDirichlet") p.bc = BCType::Dirichlet;
        else p.bc = BCType::MixedImproved;

        auto v = solveScheme(p, n);
        auto v2 = solveScheme(p, 2 * n);

        f << "Узел;x;v(x);v2(x);Разность\n";
        double eps2 = 0; double max_x = 0;
        for (int i = 0; i <= n; ++i) {
            double x = i / (double)n;
            double diff = abs(v[i] - v2[2 * i]);
            if (diff > eps2) { eps2 = diff; max_x = x; }
            f << i << ";" << x << ";" << v[i] << ";" << v2[2 * i] << ";" << diff << "\n";
        }
        cout << "=== " << (bc_str == "MainDirichlet" ? "ОСНОВНАЯ (1-я краевая)" : "ОСНОВНАЯ (Смешанная)") << " ===\n";
        cout << "n = " << n << "\n";
        cout << "eps2 = " << scientific << setprecision(6) << eps2 << " (задано: <= " << target_eps << ")\n";
        cout << "Макс. разность в x = " << fixed << setprecision(4) << max_x << "\n";
    }

    return 0;
}