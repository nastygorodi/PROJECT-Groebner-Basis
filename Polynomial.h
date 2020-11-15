#include <initializer_list>
#include <iostream>
#include <map>
#include <math.h>
#include "Modular.h"
//#include "Monomial.h"
#include "Order.h"
#include "Rational.h"


namespace Groebner {
template <class Coeff, class Order>
class Polynomial {
public:
    Polynomial() = default;

    Polynomial(std::initializer_list<std::pair<Monomial, Coeff>> terms) {
        for (auto term : terms) {
            if (term.second != 0) {
                auto result = terms_.try_emplace(term.first, term.second);
                assert(result.second);
            }
        }
    }

    Coeff coeff_of_term(const Monomial& m) const {
        auto it = terms_.find(m);
        if (it != terms_.end()) {
            return it->second;
        }
        return 0;
    }

    const std::map<Monomial, Coeff, Reverse<Order>>& get_terms() const {
        return terms_;
    }

    Polynomial& operator+=(const Polynomial& other) {
        for (const auto& term : other.terms_) {
            if (coeff_of_term(term.first) == 0) {
                auto result = terms_.try_emplace(term.first, term.second);
                assert(result.second);
            } else {
                terms_[term.first] += term.second;
            }
        }
        reduce();
        return *this;
    }

    Polynomial& operator*=(const Polynomial& factor) {
        Polynomial result;
        for (const auto& term1 : terms_) {
            for (const auto& term2 : factor.terms_) {
                auto check = result.terms_.try_emplace(term1.first * term2.first, term1.second * term2.second);
                assert(check.second);
            }
        }
        terms_ = result.terms_;
        reduce();
        return *this;
    }

    friend Polynomial operator+(const Polynomial& first, const Polynomial& second) {
        Polynomial result = first;
        result += second;
        return result;
    }

    friend Polynomial operator*(const Polynomial& first, const Polynomial& second) {
        Polynomial result = first;
        result *= second;
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const Polynomial& current) {
        int64_t count = 0;
        for (const auto& term : current.terms_) {
            ++count;
            if (term.second > 0 && count != 1) {
                out << " + ";
            }
            if (term.second != 1 && term.second > 0|| term.second != -1 && term.second < 0) {
                out << term.second;
            } else {
                if (term.second < 0 && (term.second == -1)) {
                    out << " - ";
                }
            }
            out << term.first;
        }
        return out;
    }

private:
    void reduce() {
        auto it = terms_.cbegin();
        while (it != terms_.cend()) {
            if (it ->second == 0) {
                it = terms_.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::map<Monomial, Coeff, Reverse<Order>> terms_;
};
}
