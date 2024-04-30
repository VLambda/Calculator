// math_functions.h

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <functional>
#include <array>

double summation(double start, double end, std::function<double(double)> func);
double simpson(std::function<double(double)> f, double a, double b, int n);
double lanczosln(double z);
double gamma(double z);
double erf(double x);
double sinc(double x);
double sineIntegral(double x);
double cosineIntegral(double x);

const double G = 5.0;
const std::array<double, 7> LG5N7 = {
        1.000000000189712,
        76.18009172948503,
        -86.50532032927205,
        24.01409824118972,
        -1.2317395783752254,
        0.0012086577526594748,
        -0.00000539702438713199,
};

#endif //MATH_FUNCTIONS_H
