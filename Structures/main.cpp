#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Geometry/PointVector.hpp"

using namespace std;


struct Lol {
	int h;
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	PointVector<int> v1({1, 2, 3});
	PointVector<int> v2({2, -1, 0});
	cout << v1.dotProduct(v2) << endl;
	std::unordered_map<int, int> a;
}
