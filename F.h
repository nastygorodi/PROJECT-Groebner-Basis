#include <iostream>

namespace Groebner {
class F {
using IntegralType = std::int64_t;
public:
    F() = default;

    F(IntegralType number, IntegralType n) : number_(number), n_(n) {
        assert(n_ != 0);
        reduce();
    }

    IntegralType number() const{
        return number_;
    }

    F operator+() const {
        return *this;
    }

    F operator-() const {
        return F(n_ - number_, n_);
    }

    F& operator+=(const F& other) {
        number_ += other.number_;
        reduce();
        return *this;
    }

    F& operator-=(const F& other) {
        number_ -= other.number_;
        reduce();
        return *this;
    }

    F& operator*=(const F& factor) {
        number_ *= factor.number_;
        reduce();
        return *this;
    }

    //здесь точно есть лажа, но я пока не понимаю, как это импрувнуть
    F& operator/=(const F& divider) {
        assert(divider.number_ != 0);
        number_ /= divider.number_;
        reduce();
        return *this;
    }
private:
    IntegralType number_ = 0;
    IntegralType n_ = 1;
    void reduce() {
        assert(n_ != 0);
        while (number_ < 0) {
            number_ += n_;
        }
        number_ %= n_;
    }
};

F operator+(const F& first, const F& second) {
    F result = first;
    result += second;
    return result;
}

F operator-(const F& first, const F& second) {
    F result = first;
    result -= second;
    return result;
}

F operator*(const F& first, const F& second) {
    F result = first;
    result *= second;
    return result;
}

F operator/(const F& first, const F& second) {
    F result = first;
    result /= second;
    return result;
}

bool operator==(const F& first, const F& second) {
    return first.number() == second.number();
}

bool operator!=(const F& first, const F& second) {
    return !(first == second);
}

bool operator<(const F& first, const F& second) {
    return first.number() < second.number();
}

bool operator>(const F& first, const F& second) {
    return first.number() > second.number();
}

bool operator<=(const F& first, const F& second) {
    return !(first > second);
}

bool operator>=(const F& first, const F& second) {
    return !(first < second);
}

std::ostream& operator<<(std::ostream& out, const F& current) {
    out << current.number();
    return out;
}
};
