// Report.h
// Консольный вывод: справка, таблицы 2-6, экспорт в CSV (для графиков).
#pragma once
#include "Experiments.h"
#include <string>

using namespace std;

void printTestReport(const TestResult& r, int tableStep); //Отвечает за вывод справки + таблицы 2 для тестовой задачи
void printMainReport(const MainResult& r, int tableStep); //Отвечает за вывод справки + таблицы 3 для основной задачи 
void printOrderTable(const vector<ErrorRow>& rows, bool testProblem); //Отвечает за вывод таблиц 4 и 5
void printWideTable(const vector<ErrorRow>& rows); //Отвечает за вывод таблицы 6

//Данные для будущих графиков.
void saveTestCsv(const TestResult& r, const string& file);
void saveMainCsv(const MainResult& r, const string& file);
void saveWideCsv(const vector<ErrorRow>& rows, const string& file);
