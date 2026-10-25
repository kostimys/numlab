// TridiagonalSolver.h
#pragma once
#include <vector>
#include <cmath>

using namespace std;

// Метод прогонки для СЛАУ с трехдиагональной матрицей:
// sub[i]*y[i-1] + diag[i]*y[i] + sup[i]*y[i+1] = rhs[i],  i = 0..N-1
// где: sub - поддиагональ(жлементы слева от diag), diag - главная диагональ, sup - наддиагональ(элементы справа от diag), rhs - свободные члены
// (sub[0] и sup[N-1] не используются)

//Возможные исключения: invalid_argument - пустая система или разная длина массивов
//runtime_error - нудевой ведущий элемент

vector<double> solveTridiagonal(const vector<double>& sub, const vector<double>& diag, const vector<double>& sup, const vector<double>& rhs);
