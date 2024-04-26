#pragma once
#include "Month.h"

namespace TimeStructures {
	template <class Description>
	class Year {
	private:
		size_t number;
		std::vector<Month<Description>> months;
	public:
		Year(const size_t& number) : number(number), months(count_of_month_in_year) {
			for (int i = 0; i < count_of_month_in_year; ++i) {
				months[i] = Month<Description>(i, number);
			}
		}

		size_t get_number() const {
			return this->number;
		}

		size_t get_count_of_days() const {
			return get_count_of_days_in_year(this->number);
		}

		bool add_description(const size_t& month_number, const size_t& day, Description description) {
			if (month_number >= count_of_month_in_year) {
				return false;
			}
			return months[month_number].add_description(day, description);
		}

		Description get_description(size_t day_number, size_t month_number) {
			return this->months[month_number].get_description(day_number);
		}

		bool is_high() const {
			return is_high_year(this->number);
		}
	};
}
