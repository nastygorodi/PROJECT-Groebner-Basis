#pragma once
#include <stdlib.h>

#include <iostream>

#include "../Polynomial.h"
#include "test_Monomial.h"

using namespace Groebner;

namespace test_Polynomial {

using DegLex = Sum<Deg, Lex>;
const int64_t MOD = 17;

template <class Coeff, class Order>
Polynomial<Coeff, Order> generate_Polynomial(int test_id, int rand_) {
    int num_terms = rand() % rand_ + 7;
    Polynomial<Coeff, Order> p;
    for (int i = 1; i < num_terms; ++i) {
        Monomial cur_m = test_Monomial::generate_Monomial(test_id, rand_);
        Coeff a = rand() % MOD - 7;
        if (a != Coeff(0)) {
            Polynomial<Coeff, Order> cur({{a, cur_m}});
            p += cur;
        }
    }
    return p;
}

void run() {
    std::srand(17);
    for (int test_id = 1; test_id < 70; ++test_id) {
        auto p_1 = generate_Polynomial<Rational, Lex>(test_id + 11, test_id + 17);
        auto p_2 = generate_Polynomial<Rational, Lex>(test_id + 17, test_id + 11);
        auto p_3 = generate_Polynomial<Modular<MOD>, DegLex>(test_id + 17, test_id + 11);
        auto p_4 = generate_Polynomial<Modular<MOD>, DegLex>(test_id + 11, test_id + 17);
        try {
            auto check_coeff_of = p_1.coeff_of(test_Monomial::generate_Monomial(test_id * 2, test_id));
            p_3.normalize();
            auto check_plus = p_1 + p_2;
            auto check_sub = p_3 - p_4;
            auto check_mult = p_3 * p_4;
            bool check_eq = (p_1 == p_2);
        } catch (const std::exception& e) {
            std::cout << "Test: " << test_id << "\n";
            std::cout << "p1 = " << p_1 << "\n"
                      << "p2 = " << p_2 << "\n"
                      << "p3 = " << p_3 << "\n"
                      << "p4 = " << p_4 << "\n";
        }
    }
    std::cout << "OK : all Polynomial's tests;\n";
}
}  // namespace test_Polynomial
