#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <iostream>

#include "../Algorithm.h"

using namespace Groebner;

namespace test_Algorithm {

using DegLex = Sum<Deg, Lex>;
using DegReLex = Sum<Deg, Reverse<Lex>>;

template <class Coeff, class Order>
PolySet<Coeff, Order> generate_rootN(int n) {
    PolySet<Coeff, Order> mySet;
    for (int i = 1; i < n; ++i) {
        Polynomial<Coeff, Order> cur_p;
        for (int j = 0; j < n; ++j) {
            Monomial cur_m;
            for (int k = j; k < j + i; ++k) {
                Monomial m{{k % n, 1}};
                cur_m *= m;
            }
            cur_p += cur_m;
        }
        mySet.add(cur_p);
    }
    Monomial last_m;
    for (int j = 0; j < n; ++j) {
        Monomial m{{j % n, 1}};
        last_m *= m;
    }
    Polynomial<Coeff, Order> last_p = last_m;
    last_p -= Coeff(1);
    mySet.add(last_p);
    return mySet;
}

void run() {
    std::ofstream out("Test_Results.txt");
    for (int test_id = 2; test_id < 9; ++test_id) {
        PolySet<Rational, DegLex> X = generate_rootN<Rational, DegLex>(test_id);
        clock_t start = clock();
        X = Algorithm<Rational, DegLex>::make_GB(X);
        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        out << "The time for " << test_id << " variables: " << seconds << " seconds\n\n";
        out << "Groebner Basis:\n";
        out << X << "\n";
    }
    out.close();
    std::cout << "OK : all Algorithm's tests;\n";
}
}  // namespace test_Algorithm
