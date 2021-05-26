#pragma once
#include <stdlib.h>

#include <iostream>

#include "../PolynomialSet.h"
#include "test_Polynomial.h"

using namespace Groebner;

namespace test_PolySet {

using DegLex = Sum<Deg, Lex>;
const int64_t MOD = 17;
const int64_t NUM = 17;

template <class Coeff, class Order>
PolySet<Coeff, Order> generate_PolySet(int test_id, int rand_) {
    int num_p = rand() % rand_ + 1;
    PolySet<Coeff, Order> mySet;
    for (int i = 0; i < num_p; ++i) {
        Polynomial<Coeff, Order> cur_p = test_Polynomial::generate_Polynomial<Coeff, Order>(test_id, rand_);
        if (cur_p != Coeff(0)) {
            mySet.add(cur_p);
        }
    }
    return mySet;
}

void run() {
    std::srand(17);
    for (int test_id = 1; test_id < 100; ++test_id) {
        auto X = generate_PolySet<Rational, Lex>(test_id % NUM + 7, test_id % NUM + 7);
        auto Y = generate_PolySet<Rational, Lex>(test_id % NUM + 7, test_id % NUM + 7);
        auto W = generate_PolySet<Modular<MOD>, DegLex>(test_id % NUM + 7, test_id % NUM + 7);
        auto Z = generate_PolySet<Modular<MOD>, DegLex>(test_id % NUM + 7, test_id % NUM + 7);
        try {
            X.addSet(Y);
            W.clear();
            Y.normalize();
            Z.normalize();
        } catch (const std::exception& e) {
            std::cout << "Test: " << test_id << "\n";
            std::cout << "X = " << X << "\n"
                      << "Y = " << Y << "\n"
                      << "W = " << W << "\n"
                      << "Z = " << Z << "\n";
        }
    }
    std::cout << "OK : all PolySet's tests;\n";
}
}  // namespace test_PolySet
