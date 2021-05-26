#pragma once
#include <math.h>

#include <initializer_list>
#include <iostream>
#include <map>

#include "Modular.h"
#include "Order.h"
#include "Rational.h"

namespace Groebner {
template <class Coeff, class Order>
class Polynomial {
 public:
    using TermsContainer = std::map<Monomial, Coeff, Reverse<Order>>;
    Polynomial() = default;

    Polynomial(Coeff coefficient) {
        terms_.emplace(Monomial(), std::move(coefficient));
        reduce();
    }

    Polynomial(const Monomial& m) {
        terms_.emplace(m, Coeff(1));
    }

    Polynomial(std::initializer_list<std::pair<Coeff, Monomial>> terms) {
        for (auto term : terms) {
            if (term.first != 0) {
                auto result = terms_.try_emplace(term.second, term.first);
                assert(result.second);
            }
        }
    }

    Coeff coeff_of(const Monomial& m) const {
        auto it = terms_.find(m);
        if (it != terms_.end()) {
            return it->second;
        }
        return 0;
    }

    const TermsContainer& get_terms() const {
        return terms_;
    }

    void normalize() {
        if (terms_.size() == 0) {
            return;
        }
        auto first_coeff = terms_.begin()->second;
        for (auto& term : terms_) {
            term.second /= first_coeff;
        }
    }

    Polynomial& operator+=(const Polynomial& other) {
        for (const auto& term : other.terms_) {
            terms_[term.first] += term.second;
            if (coeff_of(term.first) == 0) {
                terms_.erase(term.first);
            }
        }
        return *this;
    }

    Polynomial& operator-=(const Polynomial& other) {
        for (const auto& term : other.terms_) {
            terms_[term.first] -= term.second;
            if (coeff_of(term.first) == 0) {
                terms_.erase(term.first);
            }
        }
        return *this;
    }

    Polynomial& operator*=(const Polynomial& factor) {
        TermsContainer result;
        for (const auto& term1 : terms_) {
            for (const auto& term2 : factor.terms_) {
                result[term1.first * term2.first] += term1.second * term2.second;
            }
        }
        terms_ = std::move(result);
        reduce();
        return *this;
    }

    friend Polynomial operator+(const Polynomial& first, const Polynomial& second) {
        Polynomial result = first;
        result += second;
        return result;
    }

    friend Polynomial operator-(const Polynomial& first, const Polynomial& second) {
        Polynomial result = first;
        result -= second;
        return result;
    }

    friend Polynomial operator*(const Polynomial& first, const Polynomial& second) {
        Polynomial result = first;
        result *= second;
        return result;
    }

    friend bool operator==(const Polynomial& first, const Polynomial& second) {
        return first.terms_ == second.terms_;
    }

    friend bool operator!=(const Polynomial& first, const Polynomial& second) {
        return !(first.terms_ == second.terms_);
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& current) {
        int64_t count = 0;
        for (const auto& term : current.terms_) {
            ++count;
            if (term.second > 0 && count != 1) {
                out << " + ";
            }
            if ((term.second != 1 && term.second > 0) || (term.second != -1 && term.second < 0) || ((term.second == -1 || term.second == 1) && term.first.get_degrees().size() == 0)) {
                out << " " << term.second << " ";
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
            if (it->second == 0) {
                it = terms_.erase(it);
            } else {
                ++it;
            }
        }
    }

    TermsContainer terms_;
};
}  // namespace Groebner
