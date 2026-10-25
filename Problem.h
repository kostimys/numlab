// Problem.h
// Описание задач (основной и тестовой) для варианта задания.
#pragma once
#include <functional>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


using Func = function<double(double)>;

// Основная задача:  (k(x) u')' - q(x) u = -f(x) - основное уравнение стационарной теплопроводности, 
// x в диапазоне (0,1),  u(0)=mu1, u(1)=mu2
// Коэффициенты имеют разрыв 1-го рода в точке xi:
// слева (x в диапазоне [0, xi)) действуют k1, q1, f1; справа уже, где (x в диапазоне (xi, 1]) - k2, q2, f2.
struct MainTask {
    Func k1, k2, q1, q2, f1, f2;
    double xi  = 0.5;
    double mu1 = 0.0;
    double mu2 = 0.0;
};

// Тестовая задача: все коэффициенты кусочно-постоянные.
// Значения - предельные значения коэффициентов основной задачи в точке xi.
struct TestTask {
    double k1, k2, q1, q2, f1, f2;
    double xi, mu1, mu2;

    // Представление тестовой задачи в общем виде (для разностной схемы)
    MainTask toProblem() const;
};

// Основная задача для нашего варианта (вариант 2 из Таблицы 1).
MainTask makeMainProblem();

// Тестовая задача, построенная по основной (предельные значения при x -> xi -/+ 0).
TestTask makeTestProblem(const MainTask& mainProblem);
