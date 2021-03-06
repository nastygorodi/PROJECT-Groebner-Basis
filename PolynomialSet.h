#pragma once
#include <iostream>
#include <set>

#include "PolynomialOrder.h"

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
        for (auto& p : polynomials) {
            if (p != Coeff(0)) {
                polynomials_.emplace(std::move(p));
            }
        }
    }

    PolySet(PolyContainer polynomials) {
        polynomials_ = polynomials;
        reduce();
    }

    const PolyContainer& get_polynomials() const {
        return polynomials_;
    }

    void add(const Polynomial<Coeff, Order>& p) {
        polynomials_.emplace(p);
    }

    void addSet(const PolySet<Coeff, Order> other) {
        for (auto item : other.get_polynomials()) {
            polynomials_.emplace(item);
        }
    }

    void clear() {
        polynomials_.clear();
    }

    void exclude(typename std::set<Polynomial<Coeff, Order>, PolynomialOrder<Coeff, Order>>::const_iterator it) {
        polynomials_.erase(it);
    }

    void normalize() {
        auto it = polynomials_.begin();
        while (it != polynomials_.end()) {
            auto it_ = it;
            ++it_;
            auto tmp = polynomials_.extract(it);
            tmp.value().normalize();
            polynomials_.insert(move(tmp));
            it = it_;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const PolySet<Coeff, Order>& current) {
        for (const auto& p : current.polynomials_) {
            out << p << " ;\n";
        }
        return out;
    }

 private:
    void reduce() {
        auto it = polynomials_.cbegin();
        while (it != polynomials_.cend()) {
            if (*it == Coeff(0)) {
                it = polynomials_.erase(it);
            } else {
                ++it;
            }
        }
    }

    PolyContainer polynomials_;
};
}  // namespace Groebner
