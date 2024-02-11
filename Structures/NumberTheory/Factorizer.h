#pragma once
#include "EratosthenesSieve.h"

namespace NumberTheory {
    template <size_t size_of_one_bitset>
    class Factorizer {
    private:
        size_t n;
        std::vector<int> minimal_prime_divisors;

    public:
        Factorizer() : n(0), minimal_prime_divisors() {}

        void build(size_t n) {
            this->n = n;
            minimal_prime_divisors.assign(n, -1);
            EratosthenesSieve<size_of_one_bitset>::build_and_get_sieve_in_vector(
                n,
                [this](int i, int j) { this->_add_min_divisor(i, j); },
                [this](int i) { this->_process_prime(i); }
            );
        }

        int get_minimal_prime_divisor(size_t n) const {
            return minimal_prime_divisors[n];
        }

        std::vector<int> factorize(size_t n, bool repetitions = true) {
            std::vector<int> prime_factors;
            while (n > 1) {
                int count = 0;
                int minimal_prime_divisor = get_minimal_prime_divisor(n);
                while (n % minimal_prime_divisor == 0) {
                    n /= minimal_prime_divisor;
                    ++count;
                }
                prime_factors.insert(prime_factors.end(), ((count - 1) * repetitions + 1), minimal_prime_divisor);
            }
            return prime_factors;
        }

    private:
        void _add_min_divisor(int i, int j) {
            if (minimal_prime_divisors[j] == -1) {
                minimal_prime_divisors[j] = i;
            }
        }

        void _process_prime(int i) {
            minimal_prime_divisors[i] = i;
        }
    };
}
