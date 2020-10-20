#include <iostream>

namespace Groebner {
template < std::int64_t mod >
class Modular {
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
        return Modular(mod - number_);
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

    //жесть

    IntegralType reduce_Euclid (IntegralType a, IntegralType b, IntegralType& x, IntegralType& y) {
        if (a == 0) {
            x = 0;
            y = 1;
            return b;
        }
        IntegralType x_, y_;
        IntegralType d = reduce_Euclid(b % a, a, x_, y_);
        x = y_ - x_ * (b / a);
        y = x_;
        return d;
    }

    bool solution_Euclid (IntegralType a, IntegralType b, IntegralType c, IntegralType& x, IntegralType& y, IntegralType& g) {
        g = reduce_Euclid(a, b, x, y);
        if (c % g != 0) {
            return false;
        }
        x *= c / g;
        y *= c / g;
        return true;
    }


    Modular& operator/=(const Modular& divider) {
        assert(divider.number_ != 0);
        Modular x, y, g;
        if (solution_Euclid(divider.number_, mod, number_, x.number_, y.number_, g.number_)) {
            number_ = x.number_;
        };
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
};
