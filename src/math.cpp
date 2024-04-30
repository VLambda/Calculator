//
// Created by vedan on 4/30/2024.
//

#include "math.h"
#include <cmath>
#include <array>
#include <functional>



double summation(double start, double end, std::function<double(double)> func) {
    double sum = 0.0;
    for (double i = start; i <= end; i++) {
        sum += func(i);
    }
    return sum;
}

double simpson(std::function<double(double)> f, double a, double b, int n) {
    double h = (b - a) / n;
    double s = f(a) + f(b); // a = x_0 and b = x_2n

    for (int i = 1; i <= n; ++i) {
        double x = a + h * i;
        s += 4.0 * f(x - h / 2.0); // 4y_1, 4y_3, 4y_5, ..., 4y_{2n-1}
        if (i < n) {
            s += 2.0 * f(x); // 2y_2, 2y_4, 2y_6, ..., 2y_{2n-2}
        }
    }
    s += 4.0 * f(b - h / 2.0); // 4y_{2n}

    return s * (h / 6.0);
}


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



double lanczosln(double z) {
    z = z - 1.0;
    double base = z + G + 0.5;

    double s = LG5N7[0];
    for (size_t i = 1; i < LG5N7.size(); ++i) {
        s += LG5N7[i] / (z + static_cast<double>(i));
    }

    return std::log(std::sqrt(2.0 * M_PI)) + std::log(s) - base + std::log(base) * (z + 0.5);
}

double gamma(double z) {
    return exp(lanczosln(z));
}

double erf(double x) {
    double result = 0.0;
    for(int i = 0; i < 1000; i++) {
        result += pow(-1, i) * pow(x, 2*i+1) / ((2*i+1) * gamma(i + 1));
    }

    return result * 1.128379167095512;
}

double sinc(double x) {
    if (x==0)
        return 1;

    return sin(x)/x;
}

double sineIntegral(double x) {
    double result = 0.0;
    for(int i = 0; i < 1000; i++) {
        result += pow(-1, i) * pow(x, 2*i+1) / ((2*i+1) * gamma(2*i+2));
    }
    return result;
}

double cosineIntegral(double x) {
    double result = 0.0;
    for(int i = 0; i < 1000; i++) {
        result += pow(-1, i) * pow(x, 2*i) / ((2*i) * gamma(2*i + 1));
    }

    return log(x) + 0.57721566490153286 + result;
}
