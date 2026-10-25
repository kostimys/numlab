#pragma once
#include "Problem.h"

enum class MethodChoose {Gauss, Midpoint};

inline const char* methodName(MethodChoose c) {
    return (c == MethodChoose::Gauss) ? "Гаусс" : "Средние прямоугольники";
}

double integrate(const Func& f, double a, double b);
double Midpoint(const Func& f, double a, double b);
double integratePiece(const Func& f1, const Func& f2, double xi, double a, double b, MethodChoose choose = MethodChoose::Gauss);