// Вычисление интегралов для коэффициентов схемы (a_i, d_i, phi_i).
#pragma once
#include "Problem.h"

enum class MethodChoose {Gauss, Midpoint}; //Опция выбора метода для подсчета интегралов на будущее; выбор между Гауссом и Методом средних прямоугольников.

inline const char* methodName(MethodChoose c) {
	if (c == MethodChoose::Gauss) {
		return "Выбрана квадратура Гаусса ";
	}
	else {
		return "Выбран метод Средних Прямоугольников ";
	}
}

// Интеграл гладкой функции по [a,b]: адаптивная квадратура Гаусса (5 узлов).
// Подходит и для функций с корневой особенностью производной (sqrt(x) в нуле).
double integrate(const Func& f, double a, double b);

// Интеграл от кусочной функции, когда точка кси не совпадает с узлом: f1 на [0, xi), f2 на (xi, 1].
// Если xi попадает внутрь [a,b], отрезок делится точкой разрыва.
double integratePiece(const Func& f1, const Func& f2, double xi, double a, double b, MethodChoose choose = MethodChoose::Gauss);

//Вычисление интеграла методом средних прямоугольников, он должен (вроде) быть универсальным как для гладкой, так и для кусочной функции.
double Midpoint(const Func& f, double a, double b);