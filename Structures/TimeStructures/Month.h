#pragma once
#include "Day.h"

namespace TimeStructures {
	template <class Description>
	class Month {
	private:
		size_t number, year_number;
		std::vector<Day<Description>> days;
	public:
		Month(size_t number = 0, size_t year_number = 0) :
			number(number% count_of_month_in_year),
			year_number(year_number),
			days(get_count_of_days_in_month(number, is_high_year(year_number))) {

		}

		size_t get_number() const {
			return this->number;
		}

		size_t get_year_number() const {
			return this->year_number;
		}

		const Day<Description>& get_day(size_t number) const {
			return this->days[number];
		}

		size_t get_count_of_days() const {
			return get_count_of_days_in_month(number, is_high_year(year_number));
		}

		bool add_description(const size_t& day_number, Description description) {
			if (day_number >= this->get_count_of_days()) {
				return false;
			}
			this->days[day_number].add_description(description);
			return true;
		}

		Description get_description(size_t day_number) {
			return this->days[day_number].get_description();
		}
	};
}
