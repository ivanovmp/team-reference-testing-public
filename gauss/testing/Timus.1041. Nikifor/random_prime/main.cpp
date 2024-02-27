#include <iostream>
#include <random>
#include "../../../../utils/typedefs.hpp"
#include "../../../../number_theory/modulo.hpp"
#include "../../../../number_theory/NT.hpp"
int main() {
    std::mt19937 rng;
    int p;
    std::uniform_int_distribution<int> seg(1'000'000'000, 2'000'000'000);
    do {
        p = seg(rng);
    } while (!NT::is_prime(p));
    std::cout << p << '\n';
}
