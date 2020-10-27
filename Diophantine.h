#include<iostream>

namespace Equations {
template <class TIntegralType, class = std::enable_if_t <std::is_integral_v <TIntegralType>>>
struct GCDResult {
    TIntegralType gcd;
    TIntegralType first_coefficient;
    TIntegralType second_coefficient;
};

template <class TIntegralType, class = std::enable_if_t <std::is_integral_v <TIntegralType>>>
struct DiophantineSolution {
    TIntegralType first_coefficient;
    TIntegralType second_coefficient;
};

//solve an equation ax + by = (a, b)
template <class TIntegralType, class = std::enable_if_t <std::is_integral_v <TIntegralType>>>
GCDResult<TIntegralType> gcd_extended (TIntegralType value1, TIntegralType value2) {
    TIntegralType divider, quotient, remainder;
    if (value2 == 0) {
        return {value1, 1, 0};
    }
    TIntegralType x2 = 1;
    TIntegralType x1 = 0;
    TIntegralType y2 = 0;
    TIntegralType y1 = 1;
    while (value2 != 0) {
        TIntegralType quotient = value1 / value2;
        TIntegralType remainder = value1 - quotient * value2;
        TIntegralType x = x2 - quotient * x1;
        TIntegralType y = y2 - quotient * y1;
        value1 = value2;
        value2 = remainder;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }
    return {value1, x2, y2};
}

template <class TIntegralType, class = std::enable_if_t <std::is_integral_v <TIntegralType>>>
DiophantineSolution<TIntegralType> solve_equation (TIntegralType value1, TIntegralType value2, TIntegralType rhs) {
    auto result = gcd_extended<TIntegralType>(value1, value2);
    assert(rhs % result.gcd == 0);
    return {result.first_coefficient * rhs / result.gcd, result.second_coefficient * rhs / result.gcd};
}
}