#pragma once
#include <iostream>
#include "Diophantine.h"
#include "Modular.h"
#include "Monomial.h"
#include "Order.h"
#include "Polynomial.h"
#include "Rational.h"
#include "PolynomialSet.h"

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

    static void make_reduction(Polynomial<Coeff, Order>& g, const Polynomial<Coeff, Order>& f) {
        auto it = reduction_check(g, f).second;
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
            make_reduction(g, f);
        }
    }

    static void complete_reduction_bySet(Polynomial<Coeff, Order>& g, const PolySet<Coeff, Order>& X) {
        int count_reductions = 1;
        while (count_reductions != 0) {
            count_reductions = 0;
            for (auto& f : X.get_polynomials()) {
                auto g_old = g;
                complete_reduction(g, f);
                if (g_old != g) {
                    ++count_reductions;
                }
            }
        }
    }

    static void complete_reduction_SetBySet(PolySet<Coeff, Order>& Y, const PolySet<Coeff, Order>& X) {
        auto y_container = Y.get_polynomials();
        auto it = y_container.begin();
        while (it != y_container.end()) {
            auto it_ = it;
            ++it_;
            auto tmp = y_container.extract(it);
            complete_reduction_bySet(tmp.value(), X);
            if (tmp.value() != Coeff(0)) {
                y_container.insert(move(tmp));
            }
            it = it_;
        }
        PolySet<Coeff, Order> Y_new = y_container;
        Y = Y_new;
    }
};
}
