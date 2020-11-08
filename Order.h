#include <iostream>
#include "Monomial.h"

namespace Groebner {
class Lex {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        auto begin1 = first.get_degrees().begin();
        auto end1 = first.get_degrees().end();
        auto begin2 = second.get_degrees().begin();
        auto end2 = second.get_degrees().end();
        while (begin1 != end1 && begin2 != end2 &&
                             begin1->first == begin2->first && begin1->second == begin2->second) {
            ++begin1;
            ++begin2;
        }
        return Lex::compare(begin1, end1, begin2, end2);
    }
private:
    using Iter = std::map<Monomial::IndexType, Monomial::DegreeType>::const_iterator;
    static bool compare(Iter begin1, Iter end1, Iter begin2, Iter end2) {
        if (begin1 != end1 && begin2 != end2 && (begin1->first > begin2->first
                 || (begin1->first == begin2->first && begin1->second < begin2->second))) {
            return true;
        }
        if (begin1 == end1 && begin2 != end2) {
            return true;
        }
        return false;
    }

};

class Deg {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        return Deg::compare(first.get_degrees().begin(), first.get_degrees().end(),
                                 second.get_degrees().begin(), second.get_degrees().end());
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

    static bool compare(Iter begin1, Iter end1, Iter begin2, Iter end2) {
        Monomial::DegreeType deg1 = total_degree(begin1, end1);
        Monomial::DegreeType deg2 = total_degree(begin2, end2);
        return (deg1 < deg2);
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
    
    bool operator()(const Monomial& first, const Monomial& second) {
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
};

template <class T>
class Reverse {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        return T::cmp(second, first);
    }
};
}
