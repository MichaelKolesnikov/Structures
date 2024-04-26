#pragma once
#include <stdlib.h>
#include <vector>

namespace NumberTheory {
	class FloorLog {
	private:
		static size_t maximum_computed_logarithm;
		static std::vector<size_t> floor_log;

	public:
		/// <summary>
		/// this is lazy initialization
		/// </summary>
		/// <param name="n"></param>
		static void initialize_to_or_clear_after(size_t n) {
			floor_log.resize(n + 1);
			if (maximum_computed_logarithm < n) {
				for (size_t i = maximum_computed_logarithm + 1; i <= n; ++i) {
					floor_log[i] = floor_log[i / 2] + 1;
				}
			}
			maximum_computed_logarithm = n;
		}

		static size_t get_floor_log(size_t n) {
			FloorLog::initialize_to_or_clear_after(n);
			return floor_log[n];
		}
	};

	size_t FloorLog::maximum_computed_logarithm = 1;
	std::vector<size_t> FloorLog::floor_log = { 0, 0 };
}
