#include <iostream>
#include "Monomial.h"

namespace Groebner {
class Lex {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        auto first_degrees = first.get_degrees();
        auto second_degrees = second.get_degrees();
        auto it1 = first_degrees.begin();
        auto it2 = second_degrees.begin();
        while(it1 != first_degrees.end() && it2 != second_degrees.end() &&
                             it1->first == it2->first && it1->second == it2->second) {
            ++it1;
            ++it2;
        }
        if (it1 != first_degrees.end()) {
            if (it2 != second_degrees.end()) {
                if (it1->first < it2->first || (it1->first == it2->first && it1->second > it2->second)) {
                    return true;
                }
            } else {
                return true;
            }
        }
        return false;
}
};

class Deg {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        int64_t deg1 = 0;
        int64_t deg2 = 0;
        auto first_degrees = first.get_degrees();
        auto second_degrees = second.get_degrees();
        for (auto it1 = first_degrees.begin(); it1 != first_degrees.end(); ++it1) {
            deg1 += it1->second;
        }
        for (auto it2 = second_degrees.begin(); it2 != second_degrees.end(); ++it2) {
            deg2 += it2->second;
        }
        if (deg1 > deg2) {
            return true;
        }
        return false;
    }
};

template <class ...Ts>
class Sum;

//почему это так работает??????? без ...<T>  в нижней строке ничего не работало
//я посмотрела в интернете, но мне очень хочется послушать от тебя про эту (заморочку), ой, магию
template <class T>
class Sum<T> {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        return T::cmp(first, second);
    }
};

//тот же вопрос
template <class T1, class ...Tk>
class Sum<T1, Tk...> {
public:
    static bool cmp(const Monomial& first, const Monomial& second) {
        if (T1::cmp(first, second)) return true;
        if (T1::cmp(second, first)) return false;
        return Sum<Tk...>::cmp(first, second);
    }
};
}
