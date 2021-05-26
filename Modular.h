#pragma once
#include <iostream>
#include <numeric>

#include "Diophantine.h"

namespace Groebner {
template <std::int64_t mod>
class Modular {
    static_assert(mod > 0, "The modulus must be positive!");

 public:
    using IntegralType = std::int64_t;
    Modular() = default;

    Modular(IntegralType number) : number_(number) {
        reduce();
    }

    IntegralType canonical_remainder() const {
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
    }

 private:
    void reduce() {
        number_ = ((number_ % mod) + mod) % mod;
    }

    IntegralType number_ = 0;
};
}  // namespace Groebner
