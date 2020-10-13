#include <iostream>
#include <numeric>

int gcd(int num, int denom) {
    if (denom == 0)
        return num;
    return gcd(denom, num % denom);
}

class Rational {
private:
    int numerator_, denominator_;
    void reduce() {
        int divider_ = gcd(numerator_, denominator_);
        numerator_ /= divider_;
        denominator_ /= divider_;
        if (denominator_ < 0) {
            numerator_ *= -1;
            denominator_ *= -1;
        }
    }

public:
    Rational(int numerator = 0, int denominator = 1) {
        numerator_ = numerator;
        denominator_ = denominator;
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
};

Rational operator+(const Rational& first, const Rational& second) {
    Rational current = first;
    current += second;
    return current;
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational current = first;
    current -= second;
    return current;
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational current = first;
    current *= second;
    return current;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational current = first;
    current /= second;
    return current;
}

bool operator==(const Rational& first, const Rational& second) {
    return first.numerator() == second.numerator() &&
     first.denominator() == second.denominator();
}

bool operator!=(const Rational& first, const Rational& second) {
    return !(first == second);
}

std::ostream& operator<<(std::ostream& out, const Rational& current) {
    out << current.numerator();
    out << "/";
    out << current.denominator();
    return out;
}
