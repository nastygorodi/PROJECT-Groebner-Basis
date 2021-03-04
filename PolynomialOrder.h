#pragma once
#include <algorithm>
#include <iostream>
#include "Polynomial.h"

namespace Groebner {
template <class Coeff, class Order>
class PolynomialOrder {
public:
    static bool cmp(const Polynomial<Coeff, Order>& first, const Polynomial<Coeff, Order>& second) {
        const auto& [position1, position2] = std::mismatch(first.get_terms().begin(), first.get_terms().end(),
                                                 second.get_terms().begin(), second.get_terms().end(),
                                                 [] (const auto& f, const auto& s) {
                                                    return (!Order::cmp(f.first, s.first) && 
                                                            !Order::cmp(s.first, f.first) && f.second == s.second);
                                                    });
        if (position2 == second.get_terms().end()) {
            return true;
        }
        if (position1 == first.get_terms().end()) {
            return false;
        }
        return Order::cmp(position2->first, position1->first) ||  
                (!Order::cmp(position1->first, position2->first) && !Order::cmp(position2->first, position1->first) 
                        && position1->second > position2->second);
    }

    bool operator()(const Polynomial<Coeff, Order>& first, const Polynomial<Coeff, Order>& second) const {
        return cmp(first, second);
    }
};
}
