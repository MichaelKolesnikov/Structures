#include "NumberTheory\\NumberTheory.h"
#include "SparseTable.h"
#include <numeric>
#include <iostream>
#include <chrono>

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	std::vector<int> v = { 12, 18, 24, 36, 48 };
	SparseTable<int> table(v, [](int a, int b) {return NumberTheory::get_greatest_common_divisor(a, b);});
	cout << table.ask_value(1, 3) << endl;
	return 0;

	int n = 1e7;
	NumberTheory::EratosthenesSieve<1024> sieve;
	auto start = std::chrono::steady_clock::now();
	sieve.build(n);
	auto end = std::chrono::steady_clock::now();
	cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << endl;

	for (int i = 0; i < 40; ++i) {
		cout << i << ' ' << sieve.is_prime(i) << '\n';
	}
	cout << flush;


	n = 1e5;
	NumberTheory::Factorizer<2048> fact;
	fact.build(n);
	for (int i = 1; i < 1e5; ++i) {
		auto ff = fact.factorize(i);
		if (std::accumulate(begin(ff), std::end(ff), 1, [](int x, int y) {return x * y;}) != i) {
			cout << "Lol" << endl;
			break;
		}
	}
	cout << "Good" << endl;
	return 0;
}
