#include <iostream>
#include <vector>
#include "TimeStructures//TimeStructures.h"

using namespace std;
using namespace TimeStructures;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	int n, year_number, day_number, month_nomber;
	string month_name, day_name;
	cin >> n >> year_number;
	Year<bool> year(year_number);
	for (int i = 0; i < n; ++i) {
		cin >> day_number >> month_name;
		--day_number;
		size_t n = month_name_to_number.at(month_name);
		year.add_description(n, day_number, true);
	}
	cin >> day_name;
	bool high = year.is_high();
	day_number = day_name_to_day_week_number.at(day_name);
	vector<size_t> count_of_holidays(count_of_days_in_week, 0);
	vector<size_t> cnt(count_of_days_in_week, 0);

	size_t count_of_days = year.get_count_of_days();

	for (int i = 0; i < count_of_days; ++i) {
		auto p = get_month_and_day_number(i, high);
		size_t num = (i + day_number) % count_of_days_in_week;
		count_of_holidays[num] += year.get_description(p.second, p.first);
		++cnt[num];
	}
	/*for (int i = 0; i < count_of_days_in_week; ++i) {
		cout << count_of_holidays[i] << ' ';
	}
	cout << endl;
	for (int i = 0; i < count_of_days_in_week; ++i) {
		cout << cnt[i] << ' ';
	}
	cout << endl;*/
	size_t min_ = INT_MAX, max_ = 0;
	size_t max_day_number = 0, min_day_number = 0;
	for (int i = 0; i < count_of_days_in_week; ++i) {
		size_t cur_count = cnt[i];
		for (int j = 0; j < count_of_days_in_week; ++j) {
			if (i == j) {
				continue;
			}
			cur_count += count_of_holidays[j];
		}
		// cout << cur_count << ' ';
		if (cur_count <= min_) {
			min_ = cur_count;
			min_day_number = i;
		}
		if (cur_count >= max_) {
			max_ = cur_count;
			max_day_number = i;
		}
	}
	// cout << endl;
	cout << day_week_number_to_name[max_day_number] << ' ';
	cout << day_week_number_to_name[min_day_number] << endl;
}
