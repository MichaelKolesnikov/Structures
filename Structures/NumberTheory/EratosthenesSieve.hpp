#pragma once
#include <vector>
#include <cmath>
#include <functional>
#include <iostream>
#include <utility>
#include "../BitArray.hpp"

namespace NumberTheory {
	class EratosthenesSieve {
	private:
		size_t n;
		size_t count_of_primes;
		BitArray prime;

	public:
		EratosthenesSieve(size_t n) : n(n), prime(n, true), count_of_primes(0) {}

		void build() {
			prime.set_false(0);
			prime.set_false(1);
			for (size_t i = 2; i * i < n; ++i) {
				if (prime[i]) {
					++count_of_primes;
					for (size_t j = i * i; j < n; j += i) {
						prime.set_false(j);
					}
				}
			}
		}

		BitArray get_sieve_in_bitarray() const {
			return this->prime;
		}

		inline bool is_prime(size_t num) const {
			return prime[num];
		}

		inline size_t get_length() const {
			return this->n;
		}

		inline size_t get_count_of_primes() const {
			return this->count_of_primes;
		}

		void go_through_prime_numbers(const std::function<void(const size_t&)>& process_prime) const {
			for (size_t i = 2; i < this->get_length(); ++i) {
				if (this->is_prime(i)) {
					process_prime(i);
				}
			}
		}

		std::vector<size_t> get_prime_numbers() const {
			std::vector<size_t> answer;
			answer.reserve(this->get_count_of_primes());
			auto prime_process = [&answer](const size_t& p) {answer.push_back(p);};
			this->go_through_prime_numbers(prime_process);
			return answer;
		}

		void go_through_prime_numbers_with_their_multiples(
			const std::function<void(const size_t&)>& process_prime,
			const std::function<void(const size_t&, const size_t&)>& get_prime_and_multiple) const {
			for (size_t i = 2; i < this->get_length(); ++i) {
				if (this->is_prime(i)) {
					process_prime(i);
					for (int j = i * i; j < this->get_length(); j += i) {
						get_prime_and_multiple(i, j);
					}
				}
			}
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
		NumberTheory::EratosthenesSieve prime(n + 1);
		prime.build();

		for (size_t i = 2; i <= n; ++i) {
			if (!prime.is_prime(i)) {
				continue;
			}
			size_t j = n - i;
			if (!prime.is_prime(j)) {
				continue;
			}
			return { i, j };
		}
		return { 0, 0 };
	}

	std::vector<size_t> make_sure_that_Bertrand_postulate_is_correct(size_t n, bool search_all = false) {
		NumberTheory::EratosthenesSieve prime(2 * n + 1);
		prime.build();
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
