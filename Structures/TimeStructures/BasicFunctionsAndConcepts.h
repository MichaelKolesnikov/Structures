#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <type_traits>

namespace TimeStructures {
	static const size_t count_of_days_in_week = 7;

	static const std::string day_week_number_to_name[] = {
		"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
	};

	static const std::map<std::string, size_t> day_name_to_day_week_number = {
		{"Monday", 0},
		{"Tuesday", 1},
		{"Wednesday", 2},
		{"Thursday", 3},
		{"Friday", 4},
		{"Saturday", 5},
		{"Sunday", 6}
	};

	static const size_t count_of_month_in_year = 12;

	static const int count_of_days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	static const std::string number_to_month_name[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

	static const std::map<std::string, size_t> month_name_to_number = {
		{"January", 0},
		{"February", 1},
		{"March", 2},
		{"April", 3},
		{"May", 4},
		{"June", 5},
		{"July", 6},
		{"August", 7},
		{"September", 8},
		{"October", 9},
		{"November", 10},
		{"December", 11}
	};

	static inline bool is_high_year(const size_t& year_number) {
		return year_number % 400 == 0 || (year_number % 4 == 0 && year_number % 100 != 0);
	}

	static inline size_t get_count_of_days_in_year(const size_t& year_number) {
		return (size_t)366 - !is_high_year(year_number);
	}

	static inline size_t get_count_of_days_in_month(const size_t& month_number, bool high_year = false) {
		return count_of_days_in_month[month_number] + (month_number == 1 && high_year);
	}

	static inline std::pair<size_t, size_t> get_month_and_day_number(size_t day_number_in_year, bool high_year = false) {
		size_t sum_count_of_days = 0, month_number = 0;
		while (sum_count_of_days <= day_number_in_year) {
			sum_count_of_days += get_count_of_days_in_month(month_number, high_year);
			++month_number;
		}
		--month_number;
		sum_count_of_days -= get_count_of_days_in_month(month_number, high_year);
		return { month_number, day_number_in_year - sum_count_of_days };
	}
}
