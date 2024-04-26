#pragma once
#include <vector>
#include <optional>
#include <functional>

namespace QueryStructures {
	template <class T>
	using BinaryFunction = std::function<T(const T&, const T&)>;

	template <class T>
	using ExponentiateFunction = std::function<T(const T&, const BinaryFunction<T>&, size_t)>;

	template <class T>
	T generic_exponentiate(const T& value, const BinaryFunction<T>& f, size_t degree = 1) {
		T answer = value;
		for (size_t i = 1; i < degree; ++i) {
			answer = f(answer, value);
		}
		return answer;
	}

	template <class T>
	class RootDecomposition {
	private:
		class Block {
		private:
			std::vector<T> block;
			std::optional<T> result_of_operation;
			std::optional<T> coefficient_not_yet_used;

			BinaryFunction<T> f;
			ExponentiateFunction<T> fexp;

			void make_real_changes() {
				if (this->coefficient_not_yet_used.has_value()) {
					for (size_t i = 0; i < this->get_length(); ++i) {
						block[i] = this->f(block[i], coefficient_not_yet_used.value());
					}
					result_of_operation = f(result_of_operation.value(), this->fexp(coefficient_not_yet_used.value(), this->f, this->get_length()));
					coefficient_not_yet_used.reset();
				}
			}
		public:
			Block(const BinaryFunction<T>& f, const ExponentiateFunction<T>& fexp) : f(f), fexp(fexp) {}

			size_t get_length() const {
				return block.size();
			}

			const T& operator[](const size_t index) const {
				return this->block[index];
			}

			T get_full_result() const {
				if (!this->coefficient_not_yet_used.has_value()) {
					return result_of_operation.value();
				}
				return this->f(result_of_operation.value(), this->fexp(coefficient_not_yet_used.value(), this->f, get_length()));
			}

			T get_part_result_on(size_t l, size_t r) {
				make_real_changes();
				T answer = block[r];
				for (size_t i = l; i < r; ++i) {
					answer = f(answer, block[i]);
				}
				return answer;
			}

			void affect_entire_block(const T& x) {
				if (!this->coefficient_not_yet_used.has_value()) {
					this->coefficient_not_yet_used = x;
					return;
				}
				coefficient_not_yet_used = this->f(coefficient_not_yet_used.value(), x);
			}

			void act_on_part_of_block(size_t l, size_t r, const T& x) {
				make_real_changes();
				for (size_t i = l; i <= r; ++i) {
					block[i] = f(block[i], x);
				}
				result_of_operation = f(result_of_operation.value(), fexp(x, this->f, (r - l + 1)));
			}

			void add_new_element(const T& element) {
				this->block.push_back(element);
				if (this->get_length() == 1) {
					this->result_of_operation = element;
				}
				else {
					this->result_of_operation = this->f(this->result_of_operation.value(), element);
				}
			}
		};

		size_t length;
		std::vector<Block> blocks;
		size_t block_length, block_count;
		BinaryFunction<T> f;
		ExponentiateFunction<T> fexp;

	public:
		RootDecomposition() = delete;
		RootDecomposition(
			const std::vector<T>& a,
			const BinaryFunction<T>& f,
			const ExponentiateFunction<T>& fexp = generic_exponentiate<T>) :
			f(f),
			length(a.size()),
			fexp(fexp),
			block_length((size_t)sqrt(length)),
			block_count(0) {
			for (int i = 0; i < length; ++i) {
				int number_block_with_a_i = i / block_length;
				if (block_count == number_block_with_a_i) {
					blocks.push_back(Block(this->f, this->fexp));
					++block_count;
				}
				blocks[number_block_with_a_i].add_new_element(a[i]);
			}
		}

		size_t get_length() const {
			return this->length;
		}

		const T& operator[](const size_t index) const {
			size_t block_number = index / block_length,
				position_in_block = index % block_length;
			return this->blocks[block_number][position_in_block];
		}

		std::optional<T> get_result_on(size_t l, size_t r) {
			std::optional<T> answer;
			if (!(0 <= l && l <= r && r < this->get_length())) {
				return answer;
			}
			size_t
				left_block_number = l / block_length,
				position_in_left = l % block_length,
				right_block_number = r / block_length,
				position_in_right = r % block_length;
			if (left_block_number == right_block_number) {
				return blocks[left_block_number].get_part_result_on(position_in_left, position_in_right);
			}
			bool left_block_taken_partially = position_in_left != 0;
			bool right_block_taken_partially = position_in_right != blocks[right_block_number].get_length() - 1;
			if (left_block_taken_partially) {
				auto answer_on_left = blocks[left_block_number].get_part_result_on(position_in_left, blocks[left_block_number].get_length() - 1);
				if (answer.has_value()) {
					answer = this->f(answer.value(), answer_on_left);
				}
				else {
					answer = answer_on_left;
				}
			}
			if (right_block_taken_partially) {
				auto anwer_on_right = blocks[right_block_number].get_part_result_on(0, position_in_right);
				if (answer.has_value()) {
					answer = this->f(answer.value(), anwer_on_right);
				}
				else {
					answer = anwer_on_right;
				}
			}
			left_block_number += left_block_taken_partially;
			right_block_number -= right_block_taken_partially;
			for (size_t i = left_block_number; i <= right_block_number; ++i) {
				answer = (answer.has_value() ? f(answer.value(), blocks[i].get_full_result()) : blocks[i].get_full_result());
			}
			return answer;
		}

		void act(size_t l, size_t r, T x) {
			size_t left_block_number = l / block_length,
				position_in_left = l % block_length,
				right_block_number = r / block_length,
				position_in_right = r % block_length;
			if (left_block_number == right_block_number) {
				return blocks[left_block_number].act_on_part_of_block(position_in_left, position_in_right, x);
			}
			bool left_block_taken_partially = position_in_left != 0;
			bool right_block_taken_partially = position_in_right != blocks[right_block_number].get_length() - 1;
			if (left_block_taken_partially) {
				blocks[left_block_number].act_on_part_of_block(position_in_left, blocks[left_block_number].get_length() - 1, x);
			}
			if (right_block_taken_partially) {
				blocks[right_block_number].act_on_part_of_block(0, position_in_right, x);
			}
			left_block_number += left_block_taken_partially;
			right_block_number -= right_block_taken_partially;
			for (int i = left_block_number; i <= right_block_number; ++i) {
				blocks[i].affect_entire_block(x);
			}
		}
	};
}

