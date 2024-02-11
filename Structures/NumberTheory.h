#pragma once
#include <vector>
#include <cmath>
#include <functional>
#include <iostream>
#include "BitArray.h"

namespace NumberTheory {
    template <size_t size_of_one_bitset>
    class EratosthenesSieve {
    private:
        size_t n;
        BitArray<size_of_one_bitset> prime;

    public:
        EratosthenesSieve(size_t n = 0, bool default_value = false) : n(n) {
            prime = BitArray<size_of_one_bitset>(n, default_value);
        }

        void build(size_t n) {
            this->n = n;
            prime = build_and_get_sieve_in_vector(n);
        }

        bool is_prime(size_t num) const {
            return prime[num];
        }

        static BitArray<size_of_one_bitset> build_and_get_sieve_in_vector(
            int n,
            const std::function<void(int, int)>& get_prime_and_multiple = [](int, int) {},
            const std::function<void(int)>& process_prime = [](int) {}
        ) {
            BitArray<size_of_one_bitset> prime(n, true);
            prime[0] = prime[1] = false;
            for (int i = 2; i < n; ++i) {
                if (prime[i]) {
                    process_prime(i);
                    for (int j = 2 * i; j < n; j += i) {
                        prime[j] = false;
                        get_prime_and_multiple(i, j);
                    }
                }
            }
            return prime;
        }

        static bool is_prime_sqrt_method(size_t n) {
            if (n < 2) {
                return false;
            }
            int sq = sqrt(n);
            for (int i = 2; i <= sq; ++i) {
                if (n % i == 0) {
                    return false;
                }
            }
            return true;
        }
    };

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
