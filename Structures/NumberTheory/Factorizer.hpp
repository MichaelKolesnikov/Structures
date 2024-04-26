#pragma once
#include "EratosthenesSieve.hpp"

namespace NumberTheory {
	class Factorizer {
	private:
		size_t n;
		std::vector<size_t> minimal_prime_divisors;

	public:
		Factorizer() : n(0), minimal_prime_divisors() {}

		inline size_t get_size() const {
			return this->n;
		}

		inline bool is_prime(size_t n) const {
			return this->get_minimal_prime_divisor(n) == n;
		}

		void build(size_t n) {
			size_t recommended_max_size = (size_t)1e6;
			this->n = n;
			if (this->n > recommended_max_size) {
				this->n = recommended_max_size;
			}
			minimal_prime_divisors.assign(this->n, -1);
			NumberTheory::EratosthenesSieve sieve(this->get_size());
			sieve.build();
			sieve.go_through_prime_numbers_with_their_multiples(
				[this](size_t i) { this->_process_prime(i); },
				[this](size_t i, size_t j) { this->_add_min_divisor(i, j); }
			);
		}

		inline int get_minimal_prime_divisor(size_t n) const {
			return minimal_prime_divisors[n];
		}

		std::vector<size_t> factorize(size_t n, bool repetitions = true) {
			if (n >= this->get_size()) {
				if (NumberTheory::EratosthenesSieve::is_prime_sqrt_method(n)) {
					return { n };
				}
			}
			std::vector<size_t> prime_factors;
			size_t current_i = 0;
			while (n >= this->get_size() && current_i < this->get_size()) {
				if (this->is_prime(current_i) && n % current_i == 0) {
					size_t count = 1;
					size_t minimal_prime_divisor = current_i;
					n /= minimal_prime_divisor;
					while (n % minimal_prime_divisor == 0) {
						n /= minimal_prime_divisor;
						++count;
					}
					prime_factors.insert(prime_factors.end(), ((count - 1) * repetitions + 1), minimal_prime_divisor);
				}
				++current_i;
			}
			if (current_i >= this->get_size()) {
				if (n >= this->get_size() && prime_factors.size() > 0) {
					if (NumberTheory::EratosthenesSieve::is_prime_sqrt_method(n)) {
						prime_factors.push_back(n);
						return prime_factors;
					}
				}
				while (n >= this->get_size()) {
					if (n % current_i == 0 && NumberTheory::EratosthenesSieve::is_prime_sqrt_method(current_i)) {
						size_t count = 1;
						size_t minimal_prime_divisor = current_i;
						n /= minimal_prime_divisor;
						while (n % minimal_prime_divisor == 0) {
							n /= minimal_prime_divisor;
							++count;
						}
						prime_factors.insert(prime_factors.end(), ((count - 1) * repetitions + 1), minimal_prime_divisor);
					}
					++current_i;
				}
			}
			while (n > 1) {
				size_t count = 0;
				size_t minimal_prime_divisor = get_minimal_prime_divisor(n);
				while (n % minimal_prime_divisor == 0) {
					n /= minimal_prime_divisor;
					++count;
				}
				prime_factors.insert(prime_factors.end(), ((count - 1) * repetitions + 1), minimal_prime_divisor);
			}
			return prime_factors;
		}

	private:
		inline void _add_min_divisor(size_t i, size_t j) {
			if (minimal_prime_divisors[j] == -1) {
				minimal_prime_divisors[j] = i;
			}
		}

		inline void _process_prime(size_t i) {
			minimal_prime_divisors[i] = i;
		}
	};
}
