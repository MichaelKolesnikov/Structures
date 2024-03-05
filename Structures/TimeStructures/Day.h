#pragma once
#include "BasicFunctionsAndConcepts.h"

namespace TimeStructures {
	template <class Description>
	class Day {
	private:
		size_t number, month_number, year_number;
		Description description;
	public:
		Day(size_t number = 0, size_t month_number = 0, size_t year_number = 0, Description description = Description()) :
			number(number), month_number(month_number), year_number(year_number), description(description) {
			static_assert(std::is_default_constructible<Description>::value, "Description must be default constructible");
		}

		size_t get_number() const {
			return this->number;
		}

		size_t get_month_number() const {
			return this->month_number;
		}

		size_t get_year_number() const {
			return this->year_number;
		}

		const Description& get_description() const {
			return this->description;
		}

		void add_description(Description description) {
			this->description += description;
		}
	};
}
