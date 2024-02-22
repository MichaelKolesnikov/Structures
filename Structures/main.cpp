#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>
#include "NumberTheory//NumberTheory.h"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	size_t n;
	cin >> n;
	/*if (n > 1e5) {
		if (NumberTheory::EratosthenesSieve<1>::is_prime_sqrt_method(n)) {
			cout << n << endl;
			return 0;
		}

	}*/
	NumberTheory::Factorizer<8> factorizer;
	factorizer.build(n + 1);
	auto answer = factorizer.factorize(n);
	for (int i = 0; i < answer.size() - 1; ++i) {
		cout << answer[i] << '*';
	}
	cout << answer.back() << endl;
}
