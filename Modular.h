#include <iostream>
#include <numeric>

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
    TIntegralType divider, quotient, remainder, x, x2, x1, y, y2, y1;
    if (value2 == 0) {
        x = 1;
        y = 0;
        divider = value1;
        return {divider, x, y};
    }
    x2 = 1;
    x1 = 0;
    y2 = 0;
    y1 = 1;
    while (value2 > 0) {
        quotient = value1 / value2;
        remainder = value1 - quotient * value2;
        x = x2 - quotient * x1;
        y = y2 - quotient * y1;
        value1 = value2;
        value2 = remainder;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }
    divider = value1;
    x = x2;
    y = y2;
    return {divider, x, y};
}

template <class TIntegralType, class = std::enable_if_t <std::is_integral_v <TIntegralType>>>
DiophantineSolution<TIntegralType> solve_equation (TIntegralType value1, TIntegralType value2, TIntegralType rhs) {
    DiophantineSolution<TIntegralType> solution;
    auto result = gcd_extended<TIntegralType>(value1, value2);
    assert(rhs % result.gcd == 0);
    solution.first_coefficient = result.first_coefficient * rhs / result.gcd;
    solution.second_coefficient = result.second_coefficient * rhs / result.gcd;
    return solution;
}
}

namespace Groebner {
template < std::int64_t mod >
class Modular {
static_assert(mod > 0, "The modulus must be positive!");
public:
    using IntegralType = std::int64_t;
    Modular() = default;

    Modular(IntegralType number) : number_(number) {
        reduce();
    }

    IntegralType canonical_remainder() const{
        return number_;
    }

    Modular operator+() const {
        return *this;
    }

    Modular operator-() const {
        return Modular(-number_);
    }

    Modular& operator+=(const Modular& other) {
        number_ += other.number_;
        reduce();
        return *this;
    }

    Modular& operator-=(const Modular& other) {
        number_ -= other.number_;
        reduce();
        return *this;
    }

    Modular& operator*=(const Modular& factor) {
        number_ *= factor.number_;
        reduce();
        return *this;
    }

    Modular& operator/=(const Modular& divider) {
        auto solution = Equations::solve_equation<IntegralType>(divider.number_, mod, number_);
        number_ = solution.first_coefficient;
        reduce();
        return *this;
    }

    friend Modular operator+(const Modular& first, const Modular& second) {
        Modular result = first;
        result += second;
        return result;
    }

    friend Modular operator-(const Modular& first, const Modular& second) {
        Modular result = first;
        result -= second;
        return result;
    }

    friend Modular operator*(const Modular& first, const Modular& second) {
        Modular result = first;
        result *= second;
        return result;
    }

    friend Modular operator/(const Modular& first, const Modular& second) {
        Modular result = first;
        result /= second;
        return result;
    }

    friend bool operator==(const Modular& first, const Modular& second) {
        return first.canonical_remainder() == second.canonical_remainder();
    }

    friend bool operator!=(const Modular& first, const Modular& second) {
        return !(first == second);
    }

    friend bool operator<(const Modular& first, const Modular& second) {
        return first.canonical_remainder() < second.canonical_remainder();
    }
    
    friend bool operator>(const Modular& first, const Modular& second) {
        return second < first;
    }

    friend bool operator<=(const Modular& first, const Modular& second) {
        return !(first > second);
    }

    friend bool operator>=(const Modular& first, const Modular& second) {
        return !(first < second);
    }

    friend std::ostream& operator<<(std::ostream& out, const Modular& current) {
        return out << current.canonical_remainder();
    };
private:
    void reduce() {
        number_ = ((number_ % mod) + mod) % mod;
    }

    IntegralType number_ = 0;
};
}
