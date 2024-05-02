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
    double z = std::abs(x);
    double p1 = 0.56418958354775629/(z+2.06955023132914151);
    double p2 = (pow(z,2)+2.71078540045147805*z+5.80755613130301624)/(pow(z,2)+3.47954057099518960*z+12.0616688728623955);
    double p3 = (pow(x,2)+3.47469513777439592*z+12.07402036406381411)/(pow(z,2)+3.72068443960225092*z+8.44319781003968454);
    double p4 = (pow(z,2)+4.00561509202259545*z+9.30596659485887898)/(pow(z,2)+3.90225704029924078*z+6.36161630953880464);
    double p5 = (pow(z,2)+5.16722705817812584*z+9.12661617673673262)/(pow(z,2)+4.03296893109262491*z+5.13578530585681539);
    double p6 = (pow(z,2)+5.95908795446633271*z+9.19435612886969243)/(pow(z,2)+4.11240942957450885*z+4.48640329523408675);
    double p7 = exp(-1.0 * pow(z,2));

    double j = p1 * p2 * p3 * p4 * p5 * p6 * p7;
    double k = -1.0 * j + 1;

    if (x < 0) {
        return -1.0 * k;
    } else {
        return k;
    }
}

double sinc(double x) {
    if (x==0)
        return 1;

    return sin(x)/x;
}

double sineIntegral(double x) {
    double z = std::abs(x);
    double j = 0.0;
    if (z<=4) {
        double p1 = 1-4.54393409816329991e-2*pow(z,2)+1.15457225751016682e-3*pow(z,4)-1.41018536821330254e-5*pow(z,6)+9.43280809438713025e-8*pow(z,8)-3.53201978997168357e-10*pow(z,10)+7.08240282274875911e-13*pow(z,12)-6.05338212010422477e-16*pow(z,14);
        double p2 = 1+1.01162145739225565e-2*pow(z,2)+4.99175116169755106e-5*pow(z,4)+1.55654986308745614e-7*pow(z,6)+3.28067571055789734e-10*pow(z,8)+4.5049097575386581e-13*pow(z,10)+3.21107051193712168e-16*pow(z,12);
    j = p1/p2;
        return j;
    } else {
        return 0.0;
    }
}

double cosineIntegral(double x) {
    double result = 0.0;
    for(int i = 0; i < 1000; i++) {
        result += pow(-1, i) * pow(x, 2*i) / ((2*i) * gamma(2*i + 1));
    }

    return log(x) + 0.57721566490153286 + result;
}
