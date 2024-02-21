#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "QueryStructures//QueryStructures.h"

using namespace QueryStructures;

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	ostringstream out;
	int n, q, l, r;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		a[i] = (1 ? a[i] == 0 : 0);
		// out << a[i] << ' ';
	}
	// out << endl;
	cin >> q;
	SparseTable<int> s(a, [](const int& a, const int& b) {return a + b;});
	for (int i = 0; i < q; ++i) {
		cin >> l >> r;
		--l; --r;
		out << s.ask_value(l, r) << ' ';
	}
	cout << out.str() << std::flush;
}
