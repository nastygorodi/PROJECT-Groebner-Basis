#include "test_Algorithm.h"
#include "test_PolySet.h"

void run_all_tests() {
    test_Monomial::run();
    test_Polynomial::run();
    test_PolySet::run();
    test_Algorithm::run();
}
