//
// Created by vedan on 4/30/2024.
//

#include "token.h"
#include <regex>
#include <unordered_map>
#include <functional>
#include <string>
#include <cmath>
#include "error.h"
#include "math.h"

using namespace token;

static double factorial(double a) {
    // only positive integer values are allowed
    if (a - std::trunc(a) != 0 || a < 0)
        ARITHMETIC_ERROR("'factorial()' has to be called with positive integer argument");

    // factorial of numbers greater than 22 cannot be represented in 64 bit doubles
    if (a > 22)
        ARITHMETIC_ERROR("factorials larger than 22 cannot be computed");

    // lookup value instead of computing it
    static const double lookup_table[] = {
            1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0, 40320.0, 362880.0, 3628800.0, 39916800.0,
            479001600.0, 6227020800.0, 87178291200.0, 1307674368000.0, 20922789888000.0,
            355687428096000.0, 6402373705728000.0, 121645100408832000.0, 2432902008176640000.0,
            51090942171709440000.0, 1124000727777607680000.0
    };

    return lookup_table[(unsigned) a];
}

// single argument functions
const static std::unordered_map<std::string, func> functions = {
        {"sin", [](double a) { return std::sin(a); }},
        {"cos", [](double a) { return std::cos(a); }},
        {"tan", [](double a) { return std::tan(a); }},
        {"sqrt", [](double a) { return a >= 0 ? std::sqrt(a) : ARITHMETIC_ERROR("negative argument in 'sqrt()'"); }},
        {"cbrt", [](double a) { return std::cbrt(a); }},
        {"sqr", [](double a) { return a * a; }},
        {"log2", [](double a) { return a >= 0 ? std::log2(a) : ARITHMETIC_ERROR("negative argument in 'log2()'"); }},
        {"log10", [](double a) { return a >= 0 ? std::log10(a) : ARITHMETIC_ERROR("negative argument in 'log10()'"); }},
        {"ln", [](double a) { return a >= 0 ? std::log(a) : ARITHMETIC_ERROR("negative argument in 'ln()'"); }},
        {"exp", [](double a) { return std::exp(a); }},
        {"asin", [](double a) { return std::asin(a); }},
        {"acos", [](double a) { return std::acos(a); }},
        {"atan", [](double a) { return std::atan(a); }},
        {"sinh", [](double a) { return std::sinh(a); }},
        {"cosh", [](double a) { return std::cosh(a); }},
        {"tanh", [](double a) { return std::tanh(a); }},
        {"cot", [](double a) { return 1 / std::tan(a); }},
        {"abs", [](double a) { return std::abs(a); }},
        {"ceil", [](double a) { return std::ceil(a); }},
        {"floor", [](double a) { return std::floor(a); }},
        {"trunc", [](double a) { return std::trunc(a); }},
        {"asinh", [](double a) { return std::asinh(a); }},
        {"acosh", [](double a) { return std::acosh(a); }},
        {"atanh", [](double a) { return std::atanh(a); }},
        {"!", [](double a) { return factorial(a); }},
        {"circumference", [] (double a) { return a >= 0 ? 2.0 * M_PI * a : ARITHMETIC_ERROR("negative argument in 'circumference()'"); }},
        {"circle_area", [] (double a) { return a >= 0 ? M_PI * a * a : ARITHMETIC_ERROR("negative argument in 'circle_area()'"); }},
        {"ball_volume", [] (double a) { return a >= 0 ? (4.0 / 3.0) * M_PI * a * a * a : ARITHMETIC_ERROR("negative argument in 'ball_volume()'"); }},
        {"ball_surface", [] (double a) { return a >= 0 ? 4.0 * M_PI * a * a : ARITHMETIC_ERROR("negative argument in 'ball_surface()'"); }},
        {"sign", [](double a) { return a >= 0 ? 1 : -1 ;}},
        {"erf", [](double a) {return erf(a); }},
        {"sinc", [](double a) {return sinc(a); }},
        {"Si", [](double a) {return sineIntegral(a); }} ,
        {"Ci", [](double a) {return cosineIntegral(a); }},
        {"gamma", [](double a) {return gamma(a); }}
};

// double argument functions
const static std::unordered_map<std::string, op> operators = {
        {"+", [](double a, double b) { return a + b; }},
        {"-", [](double a, double b) { return a - b; }},
        {"*", [](double a, double b) { return a * b; }},
        {"/", [](double a, double b) { return b != 0.0 ? a / b : ARITHMETIC_ERROR("division by 0"); }},
        {"^", [](double a, double b) { return std::pow(a, b); }},
        {"max", [](double a, double b) { return a > b ? a : b; }},
        {"min", [](double a, double b) { return a < b ? a : b; }},
        {"log", [](double a, double b) { return a >= 0 and b >= 0 ? std::log(b) / std::log(a) : ARITHMETIC_ERROR("negative argument in 'log()'"); }},
        {"root", [](double a, double b) { return a >= 0 and b >= 0 ? std::pow(b, 1 / a) : ARITHMETIC_ERROR("negative argument in 'root()'"); }},
        {"mod", [](double a, double b) { return b != 0 ? std::fmod(a, b) : ARITHMETIC_ERROR("division by 0"); }},
        {"binomial", [](double a, double b) { return a >= b ? factorial(a) / (factorial(b) * factorial(a - b)) : ARITHMETIC_ERROR("n must be greater or equal to k"); }},
        {"hypot", [](double a, double b) { return a >= 0 && b >= 0 ? std::hypot(a, b) : ARITHMETIC_ERROR("negative argument in 'hypot()'"); }}
};

static std::unordered_map<std::string, double> variables = {
        {"pi", M_PI},
        {"e", M_E},
        {"inf", INFINITY}
};

bool token::is_operator(const std::string &token) {
    return operators.find(token) != operators.end();
}

bool token::is_function(const std::string &token) {
    return functions.find(token) != functions.end();
}

func token::get_function(const std::string &token) {
    return (*functions.find(token)).second;
}

op token::get_operator(const std::string &token) {
    return (*operators.find(token)).second;
}

double token::get_variable(const std::string &token) {
    return (*variables.find(token)).second;
}

void token::add_variable(const std::string &token, double val) {
    // constants must not be overwritten
    if (token == "e" or token == "pi" or token == "inf")
        throw std::runtime_error("error: '" + token +"' cannot be assigned");
    else
        variables[token] = val;
}

bool token::is_number(const std::string &token) {
    const static std::regex number("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
    return std::regex_match(token, number);
}

bool token::is_variable(const std::string &token) {
    return variables.find(token) != variables.end();
}

int token::precedence(const std::string &token) {
    if (token == "+" or token == "-") {
        return 2;
    } else if (token == "*" or token == "/") {
        return 3;
    } else if (token == "^") {
        return 4;
    } else if (is_function(token)) {
        return 5;
    } else if (token == "!") {
        return 6;
    } else {
        return 4;
    }
}

int token::associativity(const std::string &token) {
    if (token == "^")
        return RIGHT;
    else
        return LEFT;
}