// Report.cpp
#include "Report.h"
#include <cmath>
#include <cstdio>
#include <fstream>

using namespace std;



void printTestReport(const TestResult& r, int step) {
    printf("\n=== ТЕСТОВАЯ ЗАДАЧА ===\n");
    printf("Для решения задачи использована равномерная сетка с числом разбиений n = %d;\n", r.n);
    printf("задача должна быть решена с погрешностью не более eps = %.1e;\n", kEps);
    printf("интегралы для коэффициентов схемы: %s;\n", methodName(r.choose));
    printf("задача решена с погрешностью eps1 = %.6e;\n", r.eps1);
    printf("максимальное отклонение аналитического и численного решений наблюдается в точке x = %.6f\n\n", r.Max);

    printf("%8s %12s %16s %16s %14s\n", "№ узла", "x_i", "u(x_i)", "v(x_i)", "|u-v|");
    for (int i = 0; i <= r.n; i += step)
        printf("%8d %12.6f %16.10f %16.10f %14.4e\n", i, r.x[i], r.u[i], r.v[i], r.diff[i]);
    if (r.n % step != 0) //Этой строкой мы дописываем последний самый узел в случае, если наше заданное n делится на шаг сетки step и получается так, что последняя точка n не входит; ее мы тут и дописываем, так как нам важно видеть оба конца стержня.
        printf("%8d %12.6f %16.10f %16.10f %14.4e\n", r.n, r.x[r.n], r.u[r.n], r.v[r.n], r.diff[r.n]);
}

void printMainReport(const MainResult& r, int step) {
    printf("\n=== ОСНОВНАЯ ЗАДАЧА ===\n");
    printf("Для решения задачи использована равномерная сетка с числом разбиений n = %d;\n", r.n);
    printf("Задача должна быть решена с заданной точностью: eps = %.1e;\n", kEps);
    printf("Интегралы для коэффициентов схемы: %s;\n", methodName(r.choose));
    printf("Задача решена с точностью: eps2 = %.6e;\n", r.eps2);
    printf("Максимальная разность численных решений в общих узлах сетки наблюдается в точке: x = %.6f\n\n", r.Max);

    printf("%8s %12s %16s %16s %14s\n", "№ узла", "x_i", "v(x_i)", "v2(x_2i)", "|v-v2|");
    for (int i = 0; i <= r.n; i += step)
        printf("%8d %12.6f %16.10f %16.10f %14.4e\n", i, r.x[i], r.v[i], r.v2[i], r.diff[i]);
    if (r.n % step != 0)
        printf("%8d %12.6f %16.10f %16.10f %14.4e\n", r.n, r.x[r.n], r.v[r.n], r.v2[r.n], r.diff[r.n]);
}

void printOrderTable(const vector<ErrorRow>& rows, bool testProblem) {
    const char* tableN;
    const char* table;
    if (testProblem) {
        tableN = "4";
        table = "тестовая";
    }
    else {
        tableN = "5";
        table = "основная";
    }

    printf("\n=== Таблица %s: проверка порядка (%s задача) ===\n", tableN, table);

    const char* errorN;
    if (testProblem) {
        errorN = "max|u-v| (eps1)";
    }
    else {
        errorN = "max|v-v2| (eps2)";
    }
    printf("%10s %18s %18s %12s\n", "n", errorN, "отношение", "порядок");

    for (size_t i = 0; i < rows.size(); ++i) {
        double e = 0.0;
        if (testProblem) {
            e = rows[i].eps1;
        } 
        else {
            e = rows[i].eps2;
        }

        if (i == 0) {
            printf("%10d %18.6e %18s %12s\n", rows[i].n, e, "---", "---");
        } 
        else {
            const double ePrev = testProblem ? rows[i - 1].eps1 : rows[i - 1].eps2;
            const double ratio = ePrev / e;
            const double order = log(ratio) / log(static_cast<double>(rows[i].n) / rows[i - 1].n);
            printf("%10d %18.6e %18.4f %12.4f\n", rows[i].n, e, ratio, order);
        }
    }
}

void printWideTable(const vector<ErrorRow>& rows) {
    printf("\n=== Таблица 6 ===\n");
    printf("%10s %21s %24s %10s %12s %12s\n", "n", "eps1 (тест)", "eps2 (основная)", "lg n", "-lg eps1", "-lg eps2");
    for (int i = 0; i < rows.size(); ++i) {
        const ErrorRow& r = rows[i];

        int n = r.n;
        double eps1 = r.eps1;
        double eps2 = r.eps2;
        double log_n = log10(static_cast<double>(r.n));
        double log_eps1 = -log10(r.eps1);
        double log_eps2 = -log10(r.eps2);
        printf("%10d %18.6e %18.6e %8.3f %10.3f %10.3f\n", r.n, r.eps1, r.eps2, log_n, log_eps1, log_eps2);
    }
}



//Здесь ниже функции для сохранения результатов в отдельные файлы, которые в дальнейшем уже можно использовать для постоения графиков в своей библиотеке Python.
void saveTestCsv(const TestResult& r, const string& file) {
    ofstream f(file);
    f.precision(15);
    f << "i;x;u;v;diff\n";
    for (int i = 0; i <= r.n; ++i) {
        f << i << ';' << r.x[i] << ';' << r.u[i] << ';' << r.v[i] << ';' << r.diff[i] << '\n';
    }
}
void saveMainCsv(const MainResult& r, const string& file) {
    ofstream f(file);
    f.precision(15);
    f << "i;x;v;v2;diff\n";
    for (int i = 0; i <= r.n; ++i) {
        f << i << ';' << r.x[i] << ';' << r.v[i] << ';' << r.v2[i] << ';' << r.diff[i] << '\n';
    }
}
void saveWideCsv(const vector<ErrorRow>& rows, const string& file) {
    ofstream f(file);
    f.precision(15);
    f << "n;eps1;eps2;lg_n;minus_lg_eps1;minus_lg_eps2\n";
    for (const auto& r : rows) {
        f << r.n << ';' << r.eps1 << ';' << r.eps2 << ';' << log10((double)r.n) << ';' << -log10(r.eps1) << ';' << -log10(r.eps2) << '\n';
    }
}
