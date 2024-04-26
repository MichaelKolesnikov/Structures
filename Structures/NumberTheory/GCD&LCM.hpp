#pragma once

namespace NumberTheory {
	size_t gcd(size_t a, size_t b) {
		if (a == 0) {
			return b;
		}
		return NumberTheory::gcd(b % a, a);
	}

	size_t lcm(size_t a, size_t b) {
		return a * b / NumberTheory::gcd(a, b);
	}

	size_t get_greatest_common_divisor(int a, int b) {
		return gcd(abs(a), abs(b));
	}

	size_t get_greatest_common_multiple(int a, int b) {
		return lcm(abs(a), abs(b));
	}
}
