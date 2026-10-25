// Лабораторная работа №2, задание 1: первая краевая задача, метод баланса.
// Консольная версия вычислительного ядра (графики/интерфейс подключаются отдельно).
#include "Experiments.h"
#include "Report.h"
#include <iostream>
#include <algorithm>
#include "ConsoleUtf8.h"

using namespace std;

int main() {

    enableUtf8Console(); //Активирует все необходимые файлы windows.h, чтобы cmake все правильно компилровал и собирал.  
    const MainTask mainProblem = makeMainProblem();
    const TestTask testProblem = makeTestProblem(mainProblem); //Тестовая задача строится на базе основной.

    //Способ вычисления интегралов для наших задач(Гаусс или Средних прямоугольников)
    //(переключается в пункте 5).
    MethodChoose choose = MethodChoose::Gauss;


    while (true) {
        cout << "\n--- Лабораторная №2, задание 1 (вариант 2) ---\n"
                     "1 - тестовая задача (подбор n под eps = 0.5e-6)\n"
                     "2 - основная задача (подбор n под eps = 0.5e-6)\n"
                     "3 - таблицы 4 и 5 (проверка порядка)\n"
                     "4 - таблица 6 (широкий диапазон n)\n"
                     "5 - сменить способ вычисления интегралов(Гаусс / Средние прямоугольники)\n"
                     "0 - выход\n> ";
        int cmd = -1;
        if (!(cin >> cmd) || cmd == 0) break;

        if (cmd == 1) {
            const int n = findNForTest(testProblem, choose);
            if (n == 0) { 
                cout << "Точность не достигнута.\n"; 
                continue; 
            }
            const TestResult r = runTestProblem(testProblem, n, choose);
            printTestReport(r, max(1, n / 20));
            saveTestCsv(r, "test_problem.csv");
            cout << "\nДанные сохранены в test_problem.csv\n";
        } 
        
        else if (cmd == 2) {
            const int n = findNForMain(mainProblem, choose);
            if (n == 0) { 
                cout << "Точность не достигнута.\n"; 
                continue; 
            }
            const MainResult r = runMainProblem(mainProblem, n, choose);
            printMainReport(r, max(1, n / 20));
            saveMainCsv(r, "main_problem.csv");
            cout << "\nДанные сохранены в main_problem.csv\n";
        } 
        
        else if (cmd == 3) {
            const vector<int> ns = {10, 20, 40, 80, 160};
            cout << "\n Способ вычисления интегралов: " << methodName(choose) << "\n";
            const auto rows = collectErrors(testProblem, mainProblem, ns, true, true, choose);
            printOrderTable(rows, true);
            printOrderTable(rows, false);
        } 
        
        else if (cmd == 4) {
            const vector<int> ns = {10, 20, 50, 80, 100, 200, 500, 800, 1000,
                                         2000, 5000, 8000, 10000, 100000, 1000000};
            cout << "Считаю (для n = 10^6, это может занять некоторое время)...\n" << "\n Способ вычисления интегралов: " << methodName(choose) << "\n";
            const auto rows = collectErrors(testProblem, mainProblem, ns, true, true, choose);
            printWideTable(rows);
            saveWideCsv(rows, "table6.csv");
            cout << "\nДанные сохранены в table6.csv\n";
        }

        else if (cmd == 5) {
            if (choose == MethodChoose::Gauss) {
                choose = MethodChoose::Midpoint;
            }
            else {
                choose = MethodChoose::Gauss;
            }

            cout << "Был выбран способ подсчета интегралов: " << methodName(choose) << "\n";
        }
    }

    return 0;
}
