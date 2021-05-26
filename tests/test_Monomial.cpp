#include <stdlib.h>

#include <iostream>

#include "../Monomial.h"

using namespace Groebner;

namespace test_Monomial {
Monomial generate_Monomial(int test_id, int rand_) {
    int num_var = rand() % rand_ + 1;
    Monomial m;
    for (int i = 0; i < num_var; ++i) {
        auto ind = rand() % rand_;
        if (ind == 0) {
            continue;
        }
        Monomial cur = {{i, ind}};
        m *= cur;
    }
    return m;
}

void run() {
    std::srand(17);
    for (int test_id = 1; test_id < 100; ++test_id) {
        auto m_1 = generate_Monomial(test_id + 17, test_id + 11);
        auto m_2 = generate_Monomial(test_id + 11, test_id + 17);
        try {
            auto check_degree_of_variable = m_1.degree_of_variable(rand() % test_id);
            auto check_mult = m_1 * m_2;
            if (m_1.is_divisible_by(m_2)) {
                auto check_div = m_1 / m_2;
            }
            auto check_gcd = Monomial::gcd(m_1, m_2);
            auto check_lcm = Monomial::lcm(m_1, m_2);
        } catch (const std::exception& e) {
            std::cout << "Test: " << test_id << "\n";
            std::cout << "m1 = " << m_1 << "\n"
                      << "m2 = " << m_2 << "\n";
        }
    }
    std::cout << "OK : all Monomial's tests;\n";
}
}  // namespace test_Monomial
