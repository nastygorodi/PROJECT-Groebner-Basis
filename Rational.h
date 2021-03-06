#pragma once
#include <iostream>
#include <numeric>

namespace Groebner {
class Rational {
 public:
    using IntegralType = std::int64_t;
    Rational() = default;

    Rational(IntegralType numerator) : numerator_(numerator) {
        reduce();
    }

    Rational(IntegralType numerator, IntegralType denominator) : numerator_(numerator), denominator_(denominator) {
        assert(denominator_ != 0);
        reduce();
    }

    IntegralType numerator() const {
        return numerator_;
    }

    IntegralType denominator() const {
        return denominator_;
    }

    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        return Rational(-numerator_, denominator_);
    }

    Rational& operator+=(const Rational& other) {
        numerator_ = numerator_ * other.denominator_ + denominator_ * other.numerator_;
        denominator_ *= other.denominator_;
        reduce();
        return *this;
    }

    Rational& operator-=(const Rational& other) {
        numerator_ = numerator_ * other.denominator_ - denominator_ * other.numerator_;
        denominator_ *= other.denominator_;
        reduce();
        return *this;
    }

    Rational& operator*=(const Rational& factor) {
        numerator_ *= factor.numerator_;
        denominator_ *= factor.denominator_;
        reduce();
        return *this;
    }

    Rational& operator/=(const Rational& divider) {
        assert(divider.numerator_ != 0);
        numerator_ *= divider.denominator_;
        denominator_ *= divider.numerator_;
        reduce();
        return *this;
    }

    friend Rational operator+(const Rational& first, const Rational& second) {
        Rational result = first;
        result += second;
        return result;
    }

    friend Rational operator-(const Rational& first, const Rational& second) {
        Rational result = first;
        result -= second;
        return result;
    }

    friend Rational operator*(const Rational& first, const Rational& second) {
        Rational result = first;
        result *= second;
        return result;
    }

    friend Rational operator/(const Rational& first, const Rational& second) {
        Rational result = first;
        result /= second;
        return result;
    }

    friend bool operator==(const Rational& first, const Rational& second) {
        return first.numerator() == second.numerator() && first.denominator() == second.denominator();
    }

    friend bool operator!=(const Rational& first, const Rational& second) {
        return !(first == second);
    }

    friend bool operator<(const Rational& first, const Rational& second) {
        return (first - second).numerator() < 0;
    }

    friend bool operator>(const Rational& first, const Rational& second) {
        return second < first;
    }

    friend bool operator<=(const Rational& first, const Rational& second) {
        return !(first > second);
    }

    friend bool operator>=(const Rational& first, const Rational& second) {
        return !(first < second);
    }

    friend std::ostream& operator<<(std::ostream& out, const Rational& current) {
        out << current.numerator();
        if (current.denominator() != 1) {
            out << "/";
            out << current.denominator();
        }
        return out;
    }

 private:
    void reduce() {
        IntegralType divider_ = std::gcd(numerator_, denominator_);
        numerator_ /= divider_;
        denominator_ /= divider_;
        if (denominator_ < 0) {
            numerator_ *= -1;
            denominator_ *= -1;
        }
    }

    IntegralType numerator_ = 0;
    IntegralType denominator_ = 1;
};
}  // namespace Groebner
