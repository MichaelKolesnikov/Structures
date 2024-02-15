#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

using namespace std;

template <class T>
class RootDecomposition {
private:
	class Block {
	private:
		vector<T> block;
		T result_of_operation, add;

		void make_real_changes() {
			for (size_t i = 0; i < get_length(); ++i) {
				block[i] += add;
			}
			result_of_operation += add * get_length();
			add = T();
		}
	public:
		Block() : result_of_operation(T()), add(T()) {}

		size_t get_length() const {
			return block.size();
		}

		void build() {
			result_of_operation = accumulate(block.begin(), block.end(), T());
		}

		T get_full_result() {
			return result_of_operation + add * get_length();
		}

		T get_part_result_on(size_t l, size_t r) {
			make_real_changes();
			T ans = T();
			for (size_t i = l; i <= r; ++i) {
				ans += block[i];
			}
			return ans;
		}

		void affect_entire_block(T x) {
			add += x;
		}

		void act_on_part_of_block(size_t l, size_t r, T x) {
			make_real_changes();
			for (size_t i = l; i <= r; ++i) {
				block[i] += x;
			}
			result_of_operation += (r - l + 1) * x;
		}

		void add_new_element(T element) {
			this->block.push_back(element);
		}
	};

	size_t length;
	std::vector<Block> blocks;
	size_t block_length, block_count;

public:
	RootDecomposition() = delete;
	RootDecomposition(const vector<T>& a) : length(a.size()) {
		block_length = (size_t)sqrt(length);
		block_count = 0;
		for (int i = 0; i < length; ++i) {
			int number_block_with_a_i = i / block_length;
			if (block_count == number_block_with_a_i) {
				blocks.push_back(Block());
				++block_count;
			}
			blocks[number_block_with_a_i].add_new_element(a[i]);
		}
		for (int i = 0; i < block_count; ++i) {
			blocks[i].build();
		}
	}
	T get_result_on(size_t l, size_t r) {
		size_t left_block_number = l / block_length, position_in_left = l % block_length;
		size_t right_block_number = r / block_length, position_in_right = r % block_length;
		if (left_block_number == right_block_number) {
			return blocks[left_block_number].get_part_result_on(position_in_left, position_in_right);
		}
		bool left_block_taken_partially = position_in_left != 0;
		bool right_block_taken_partially = position_in_right != blocks[right_block_number].get_length() - 1;
		T answer = T();
		if (left_block_taken_partially) {
			answer += blocks[left_block_number].get_part_result_on(position_in_left, blocks[left_block_number].get_length() - 1);
		}
		if (right_block_taken_partially) {
			answer += blocks[right_block_number].get_part_result_on(0, position_in_right);
		}
		left_block_number += left_block_taken_partially;
		right_block_number -= right_block_taken_partially;
		for (size_t i = left_block_number; i <= right_block_number; ++i) {
			answer += blocks[i].get_full_result();
		}
		return answer;
	}
	void act(size_t l, size_t r, T x) {
		size_t posl = l / block_length, posinl = l % block_length;
		size_t posr = r / block_length, posinr = r % block_length;
		if (posl == posr) {
			blocks[posl].act_on_part_of_block(posinl, posinr, x);
			return;
		}
		if (posinl != 0) {
			blocks[posl].act_on_part_of_block(posl, blocks[posl].get_length() - 1, x);
		}
		++posl;
		if (posinr != blocks[posr].get_length() - 1) {
			blocks[posr].act_on_part_of_block(0, posinr, x);
			--posr;
		}
		for (int i = posl; i <= posr; ++i) {
			blocks[i].affect_entire_block(x);
		}
	}
};
