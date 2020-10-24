#include <initializer_list>
#include <iostream>
#include <map>

namespace Groebner {
class Monomial {
public:
    using IntegralType = std::int64_t;

    Monomial () = default;

    Monomial (std::initializer_list<std::pair<IntegralType, IntegralType>> list) {
        for (auto pair : list) {
            degrees_[pair.first] = pair.second;
        }
    }

    Monomial& operator*=(const Monomial& factor) {
        for (auto& degree : factor.degrees_) {
            (*this).degrees_[degree.first] += degree.second;
        }
        return *this;
    }

    bool operator|(Monomial& other) const{
        for (auto& degree : (*this).degrees_) {
            if (other.degrees_[degree.first] < degree.second) {
                return false;
            }
        }
        return true;
    }

    Monomial& operator/=(const Monomial& divider) {
        assert(divider | (*this));
        for (auto& degree : divider.degrees_) {
            (*this).degrees_[degree.first] -= degree.second;
        }  
        return *this;
    }

    friend Monomial gcd_of_monomials(Monomial& first, Monomial& second) {
        Monomial result;
        for (auto& degree : first.degrees_) {
            result.degrees_[degree.first] = std::min(degree.second, second.degrees_[degree.first]);
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, const Monomial& current) {
        for (auto& degree : current.degrees_) {
            if (degree.second != 0) {
                out << "x_" << degree.first;
                if (degree.second != 1) {
                    out << "^" << degree.second;
                }
            }
        }
        return out;
    };

private:
    std::map<IntegralType, IntegralType> degrees_;
};

Monomial operator*(const Monomial& first, const Monomial& second) {
    Monomial result = first;
    result *= second;
    return result;
}
Monomial operator/(const Monomial& first, const Monomial& second) {
    Monomial result = first;
    result /= second;
    return result;
}
}
