#pragma once
#include <iostream>
#include "Diophantine.h"
#include "Modular.h"
#include "Monomial.h"
#include "Order.h"
#include "Polynomial.h"
#include "Rational.h"

namespace Groebner {
template <class Coeff, class Order>
class Algorithm {
public:
    using TermsContainer = std::map<Monomial, Coeff, Reverse<Order>>;
    static Monomial L(const Polynomial<Coeff, Order>& p) {
        Monomial result;
        auto terms = p.get_terms();
        if (terms.begin() != terms.end()) {
            result = terms.begin()->first;
        }
        return result;
    }

    static Polynomial<Coeff, Order> S(const Polynomial<Coeff, Order>& f1, const Polynomial<Coeff, Order>& f2) {
        auto m = Monomial::lcm(L(f1), L(f2));
        auto m1 = m / L(f1);
        auto m2 = m / L(f2);
        Polynomial<Coeff, Order> result = f2.coeff_of(L(f2)) * (m1 * f1) - f1.coeff_of(L(f1)) * (m2 * f2);
        return result;
    }

    static void elementary_reduction(Polynomial<Coeff, Order>& g, const Polynomial<Coeff, Order>& f) {
        auto it = g.get_terms().begin();
        while ((it != g.get_terms().end()) && (!(it->first).is_divisible_by(L(f)))) {
            ++it;
        }
        if (it != g.get_terms().end()) {
            auto m1 = it->first / L(f);
            g = f.coeff_of(L(f)) * g - it->second * (m1 * f);
        }
    }

    static std::pair<bool, typename std::map<Monomial, Coeff, Reverse<Order>>::const_iterator> reduction_check(const Polynomial<Coeff, Order>& g, const Polynomial<Coeff, Order>& f) {
        auto it = g.get_terms().begin();
        while ((it != g.get_terms().end()) && (!(it->first).is_divisible_by(L(f)))) {
            ++it;
        }

        if (it == g.get_terms().end()) {
            return {false, it};
        }
        return {true, it};
    }

    static void complete_reduction(Polynomial<Coeff, Order>& g, const Polynomial<Coeff, Order>& f) {
        while (reduction_check(g, f).first == true) {
            elementary_reduction(g, f);
        }
    }
};
}
