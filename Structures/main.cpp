#include "NumberTheory\\NumberTheory.h"
#include "SparseTable.h"
#include "SegmentTree.h"
#include <numeric>
#include <iostream>
#include <sstream>
#include "RootDecomposition.h"

using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	int n;
	cin >> n;
	vector<int> a(n);
	for (int& i : a)
		cin >> i;
	RootDecomposition<int> s(a);
	int q;
	cin >> q;
	for (int _ = 0; _ < q; ++_) {
		char type;
		cin >> type;
		if (type == '?') {
			int l, r;
			cin >> l >> r;
			cout << s.get_result_on(l, r) << endl;
		}
		else {
			int l, r, x;
			cin >> l >> r >> x;
			s.act(l, r, x);
		}
	}
	return 0;

	/*ostringstream answer;
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
	cout << answer.str() << endl;*/
}
