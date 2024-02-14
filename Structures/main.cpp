#include "NumberTheory\\NumberTheory.h"
#include "SparseTable.h"
#include "SegmentTree.h"
#include <numeric>
#include <iostream>
#include <sstream>

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	ostringstream answer;
	int n, k, l, r, position;
	int value;
	char q;
	cin >> n;
	std::vector<int> a(n);
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}
	SegmentTree<int> st(a, [](int a, int b) {return a + b;});
	cin >> k;
	for (int _ = 0; _ < k; ++_) {
		cin >> q;
		if (q == 's') {
			cin >> l >> r;
			--l; --r;
			answer << st.ask_value_on(l, r) << ' ';
		}
		else {
			cin >> position >> value;
			--position;
			st.change_value(position, value);
		}
	}
	cout << answer.str() << endl;
}

/*
5
1 2 3 4 5
7
s 1 4
u 3 10
s 2 5
s 1 3
u 1 13
s 3 5
s 1 2
*/