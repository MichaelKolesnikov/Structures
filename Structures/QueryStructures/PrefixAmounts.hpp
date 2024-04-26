#pragma once
#include <vector>

namespace QueryStructures {
	template <class T>
	class PrefixAmounts {
	private:
		std::vector<T> prefix_amounts;
	public:
		PrefixAmounts() = delete;
		PrefixAmounts(const std::vector<T>& container) {
			static_assert(std::is_same<decltype(T() + T()), T>::value, "Type T must have operator+");
			prefix_amounts.resize(container.size());
			prefix_amounts[0] = container[0];
			for (int i = 1; i < prefix_amounts.size(); ++i) {
				prefix_amounts[i] = prefix_amounts[i - 1] + container[i];
			}
		}

		size_t size() const {
			return this->prefix_amounts.size();
		}

		T ask(size_t left, size_t right) {
			return this->prefix_amounts[right] - (left == 0 ? 0 : this->prefix_amounts[left - 1]);
		}
	};
}
