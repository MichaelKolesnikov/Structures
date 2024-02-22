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

        void build(
            size_t n,
            const std::function<void(size_t, size_t)>& get_prime_and_multiple = [](size_t, size_t) {},
            const std::function<void(size_t)>& process_prime = [](size_t) {}
        ) {
            this->n = n;
            prime = build_and_get_sieve_in_vector(n, get_prime_and_multiple, process_prime);
        }

        bool is_prime(size_t num) const {
            return prime[num];
        }

        static BitArray<size_of_one_bitset> build_and_get_sieve_in_vector(
            size_t n,
            const std::function<void(size_t, size_t)>& get_prime_and_multiple = [](size_t, size_t) {},
            const std::function<void(size_t)>& process_prime = [](size_t) {}
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
            if (n == 2) {
                return true;
            }
            if (n % 2 == 0) {
                return false;
            }
            int sq = sqrt(n);
            for (int i = 3; i <= sq; i += 2) {
                if (n % i == 0) {
                    return false;
                }
            }
            return true;
        }
    };

    std::pair<size_t, size_t> get_Goldbach_expansion(size_t n) {
        NumberTheory::EratosthenesSieve<8> prime;
        prime.build(n + 1);

        for (size_t i = 2; i <= n; ++i) {
            if (!prime.is_prime(i)) {
                continue;
            }
            size_t j = n - i;
            if (!prime.is_prime(j)) {
                continue;
            }
            return {i, j};
        }
        return { 0, 0 };
    }

    std::vector<size_t> make_sure_that_Bertrand_postulate_is_correct(size_t n, bool search_all = false) {
        NumberTheory::EratosthenesSieve<8> prime;
        prime.build(2 * n + 1);
        std::vector<size_t> primes;
        size_t j = n + 1;
        for (; j < 2 * n; ++j) {
            if (prime.is_prime(j)) {
                primes.push_back(j);
                if (!search_all) {
                    return primes;
                }
                break;
            }
        }
        ++j;
        for (; j < 2 * n; ++j) {
            if (prime.is_prime(j)) {
                primes.push_back(j);
            }
        }
        return primes;
    }
}
