#pragma once
#include <vector>
#include <cmath>
#include <functional>
#include <iostream>
#include "..\\BitArray.h"

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
}
