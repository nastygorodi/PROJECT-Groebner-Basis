#pragma once
#include <iostream>
#include "Polynomial.h"
#include "PolynomialOrder.h"
#include <set>

namespace Groebner {

template <class Coeff, class Order>
class PolySet {
public:
    using PolyContainer = std::set<Polynomial<Coeff, Order>, PolynomialOrder<Coeff, Order>>;
    PolySet() = default;

    PolySet(const Polynomial<Coeff, Order>& p) {
        polynomials_.emplace(p);
    }

    PolySet(std::initializer_list<Polynomial<Coeff, Order>> polynomials) {
        for (auto p : polynomials) {
            if (p != 0) {
                polynomials_.emplace(p);
            }
        }
    }

    const PolyContainer& get_polynomials() const {
        return polynomials_;
    }

    void add(const Polynomial<Coeff, Order>& p) {
        polynomials_.emplace(p);
    }

    friend std::ostream& operator<<(std::ostream& out, const PolySet<Coeff, Order>& current) {
        for (const auto& p : current.polynomials_) {
            out << p << " ; ";
        }
        return out;
    }

private:
    PolyContainer polynomials_;
};
}