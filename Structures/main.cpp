#include <iostream>
#include "NumberTheory//NumberTheory.h"

int main() {
	int n;
	int count = 0;
	std::cin >> n;
	auto f = [&count](const size_t& p) { ++count;};
	NumberTheory::SegmentedWheel sieve(n + 1);
	sieve.ListPrimes(f); 
	std::cout << count << std::endl;
}