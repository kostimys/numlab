// DifferenceScheme.h
// Однородная консервативная разностная схема (метод баланса), Модуль 4.
//
//   v0 = mu1,  vn = mu2,
//   (a_{i+1}(v_{i+1}-v_i) - a_i(v_i-v_{i-1})) / h^2 - d_i v_i = -fi_i,  i=1..n-1
//
//   a_i = ((1/h) * int_{x_{i-1}}^{x_i} dx/k(x))^(-1),   i = 1..n
//   d_i = (1/h) * int_{x_{i-0.5}}^{x_{i+0.5}} q(x) dx,  i = 1..n-1
//   fi_i = (1/h) * int_{x_{i-0.5}}^{x_{i+0.5}} f(x) dx,  i = 1..n-1

#pragma once
#include "Problem.h"
#include "Quadrature.h"
#include <vector>

using namespace std;

struct SchemeCoef {
    vector<double> a;   // для индексов 1..n   (a[0] не используется)
    vector<double> d;   // для индексов 1..n-1
    vector<double> fi;  // для индексов 1..n-1
};

// Коэффициенты схемы на сетке размерности n (учитывается точка разрыва xi)
SchemeCoef buildCoef(const MainTask& p, int n, MethodChoose choose = MethodChoose::Gauss);

// Решение разностной схемы прогонкой. Возвращает вектор v[0..n]
vector<double> solveScheme(const MainTask& p, int n, MethodChoose choose = MethodChoose::Gauss);
