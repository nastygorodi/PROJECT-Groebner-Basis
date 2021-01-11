#pragma once
#include <algorithm>
#include <iostream>
#include "Monomial.h"

namespace Groebner {
class Lex {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        const auto& [position1, position2] = std::mismatch(first.get_degrees().begin(), first.get_degrees().end(),
                                                 second.get_degrees().begin(), second.get_degrees().end());
        if (position2 == second.get_degrees().end()) {
            return false;
        }
        if (position1 == first.get_degrees().end()) {
            return true;
        }
        return position1->first > position2->first ||  (position1->first == position2->first && position1->second < position2->second);
    }

    bool operator()(const Monomial& first, const Monomial& second) const {
        return cmp(first, second);
    }
};

class Deg {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        auto deg1 = total_degree(first.get_degrees().begin(), first.get_degrees().end());
        auto deg2 = total_degree(second.get_degrees().begin(), second.get_degrees().end());
        return deg1 < deg2;
    }

    bool operator()(const Monomial& first, const Monomial& second) const {
        return cmp(first, second);
    }

private:
    using Iter = std::map<Monomial::IndexType, Monomial::DegreeType>::const_iterator;
    static Monomial::DegreeType total_degree(Iter it1, Iter it2) {
        Monomial::DegreeType res = 0;
        while (it1 != it2) {
            res += it1->second;
            ++it1;
        }
        return res;
    }
};

template <class ...Ts>
class Sum;

template <class T>
class Sum<T> {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        return T::cmp(first, second);
    }
    
    bool operator()(const Monomial& first, const Monomial& second) const {
        return cmp(first, second);
    }
};

template <class T1, class ...Tk>
class Sum<T1, Tk...> {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        if (T1::cmp(first, second)) return true;
        if (T1::cmp(second, first)) return false;
        return Sum<Tk...>::cmp(first, second);
    }

    bool operator()(const Monomial& first, const Monomial& second) const {
        return cmp(first, second);
    }
};

template <class T>
class Reverse {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        return T::cmp(second, first);
    }
    
    bool operator()(const Monomial& first, const Monomial& second) const {
        return cmp(first , second);
    }
};
}
