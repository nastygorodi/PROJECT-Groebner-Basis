#pragma once
#include "PolynomialSet.h"
#include "Algorithm.h"

namespace Groebner {
template <class Coeff, class Order>
void autoreduction(PolySet<Coeff, Order>& X) {
    int count_reductions = 1;
    PolySet<Coeff, Order> Y;
    while (count_reductions != 0) {
        count_reductions = 0;
        auto it = X.get_polynomials().begin();
        while (it != X.get_polynomials().end()) {
            auto it_ = it;
            ++it_;
            auto tmp_old = *it;
            auto tmp = *it;
            X.exclude(*it);
            Algorithm<Coeff, Order>::complete_reduction_bySet(tmp, X);
            Algorithm<Coeff, Order>::complete_reduction_bySet(tmp, Y);
            if (tmp != Coeff(0)) {
                Y.add(tmp);
            }
            if (tmp != tmp_old) {
                ++count_reductions;
            }
            it = it_;
        }
        std::swap(X, Y);
    }
    X.normalize();
}

template <class Coeff, class Order>
PolySet<Coeff, Order> make_GB(PolySet<Coeff, Order>& X) {
    autoreduction<Coeff, Order>(X);
    PolySet<Coeff, Order> Y = X;
    while (Y.get_polynomials().size() != 0) {
        X.addSet(Y);
        autoreduction<Coeff, Order>(X);
        Y.clear();
        auto it1 = X.get_polynomials().begin();
        while (it1 != X.get_polynomials().end()) {
            auto f1 = *it1;
            auto it2 = it1;
            ++it2;
            while (it2 != X.get_polynomials().end()) {
                auto f2 = *it2;
                auto S_ij = Algorithm<Coeff, Order>::S(f1, f2);
                Algorithm<Coeff, Order>::complete_reduction_bySet(S_ij, X);
                if (S_ij != Coeff(0)) {
                    Y.add(S_ij);
                }
                ++it2;
            }
            ++it1;
        }
    }
    autoreduction<Coeff, Order>(X);
    return X;
}
}
