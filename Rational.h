#include <iostream>
#include <numeric>

using namespace Groebner;

namespace Groebner {
class Rational {
public:
    Rational() = default;

    Rational(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        assert(denominator_ != 0);
        reduce();
    }

    int numerator() const {
        return numerator_;
    }

    int denominator() const {
        return denominator_;
    }


    Rational operator+() const {
        Rational x;
        x.numerator_ = numerator_;
        x.denominator_ = denominator_;
        return x;
    }

    Rational operator-() const {
        Rational x;
        x.numerator_ = -numerator_;
        x.denominator_ = denominator_;
        return x;
    }

    Rational& operator+=(const Rational& addendum) {
        numerator_ = numerator_ * addendum.denominator_ + denominator_ * addendum.numerator_;
        denominator_ *= addendum.denominator_;
        reduce();
        return *this;
    }

    Rational& operator-=(const Rational& subtrahend) {
        numerator_ = numerator_ * subtrahend.denominator_ - denominator_ * subtrahend.numerator_;
        denominator_ *= subtrahend.denominator_;
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
        int cur = divider.numerator_;
        numerator_ *= divider.denominator_;
        denominator_ *= cur;
        reduce();
        return *this;
    }

    Rational& operator++() {
        numerator_ += denominator_;
        return *this;
    }

    Rational& operator--() {
        numerator_ -= denominator_;
        return *this;
    }

    Rational operator++(int) {
        Rational cur = *this;
        ++*this;
        return cur;
    }

    Rational operator--(int) {
        Rational cur = *this;
        --*this;
        return cur;
    }
private:
int numerator_ = 0;
int denominator_ = 1;
void reduce() {
    int divider_ = std::gcd(numerator_, denominator_);
    numerator_ /= divider_;
    denominator_ /= divider_;
    if (denominator_ < 0) {
        numerator_ *= -1;
        denominator_ *= -1;
    }
}
};

Rational operator+(const Rational& first, const Rational& second) {
    Rational result = first;
    result += second;
    return result;
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational result = first;
    result -= second;
    return result;
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational result = first;
    result *= second;
    return result;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational result = first;
    result /= second;
    return result;
}

bool operator==(const Rational& first, const Rational& second) {
    return first.numerator() == second.numerator() &&
     first.denominator() == second.denominator();
}

bool operator!=(const Rational& first, const Rational& second) {
    return !(first == second);
}

bool operator<(const Rational& first, const Rational& second) {
    if ((first - second).numerator() < 0) {
        return true;
    } else {
        return false;
    }
}

bool operator>(const Rational& first, const Rational& second) {
    if ((first - second).numerator() > 0) {
        return true;
    } else {
        return false;
    }
}

bool operator<=(const Rational& first, const Rational& second) {
    return !(first > second);
}

bool operator>=(const Rational& first, const Rational& second) {
    return !(first < second);
}

std::ostream& operator<<(std::ostream& out, const Rational& current) {
    out << current.numerator();
    out << "/";
    out << current.denominator();
    return out;
};
}
