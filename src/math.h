// math_functions.h

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <cmath>
#include <functional>
#include <array>

class math{

};

double summation(double start, double end, std::function<double(double)> func);
double simpson(std::function<double(double)> f, double a, double b, int n);
double lanczosln(double z);
double gamma(double z);
double erf(double x);
double sinc(double x);
double sineIntegral(double x);
double cosineIntegral(double x);

#endif //MATH_FUNCTIONS_H
