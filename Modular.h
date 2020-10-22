#include <iostream>
#include <numeric>
#include <optional>

namespace Equations {
using IntegralType = std::int64_t;

//solve an equation ax + by = (a, b)
IntegralType gcd_extended (IntegralType coeff_1, IntegralType coeff_2, IntegralType& x, IntegralType& y) {
    IntegralType divider, quotient, remainder, x2, x1, y2, y1;
    if (coeff_2 == 0) {
        x = 1;
        y = 0;
        divider = coeff_1;
        return divider;
    }
    x2 = 1;
    x1 = 0;
    y2 = 0;
    y1 = 1;
    while (coeff_2 > 0) {
        quotient = coeff_1 / coeff_2;
        remainder = coeff_1 - quotient * coeff_2;
        x = x2 - quotient * x1;
        y = y2 - quotient * y1;
        coeff_1 = coeff_2;
        coeff_2 = remainder;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }
    divider = coeff_1;
    x = x2;
    y = y2;
    return divider;
}

std::optional<std::pair<IntegralType, IntegralType>> solve_equation (IntegralType coeff_1, IntegralType coeff_2, IntegralType coeff_3, IntegralType& param_1, IntegralType& param_2) {
    IntegralType divider = gcd_extended(coeff_1, coeff_2, param_1, param_2);
    //если (a, b) не делит const --> реш-й не сущ.
    if (coeff_3 % divider != 0) {
        return {};
    }
    //находили для ax + by = (a, b) --> чтобы получить решение диофантова ур-я \cdot const/(a, b)
    param_1 *= coeff_3 / divider;
    param_2 *= coeff_3 / divider;
    return std::pair(param_1, param_2);
}
}

namespace Groebner {
using namespace Equations;

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
        assert(std::gcd(divider.number_, mod) == 1);
        IntegralType param_1, param_2;
        std::optional<std::pair<IntegralType, IntegralType>> solution = solve_equation(divider.number_, mod, number_, param_1, param_2);
        if (solution != std::nullopt) {
            number_ = (*solution).first;
        }
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
