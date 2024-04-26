#pragma once
#include <cstring>

typedef int container;
const container degrees[] = { (1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5), (1 << 6), (1 << 7),
						   (1 << 8), (1 << 9), (1 << 10), (1 << 11), (1 << 12), (1 << 13), (1 << 14),
						   (1 << 15), (1 << 16), (1 << 17), (1 << 18), (1 << 19), (1 << 20), (1 << 21),
						   (1 << 22), (1 << 23), (1 << 24), (1 << 25), (1 << 26), (1 << 27), (1 << 28),
						   (1 << 29), (1 << 30), (1 << 31) };
const int main_degree = (sizeof(container) == 1 ? 3 : (sizeof(container) == 4 ? 5 : (sizeof(container) == 8 ? 6 : 0)));
const container for_mod = degrees[main_degree] - 1;
const container one = 1;

class BitArray {
private:
	int _size;
	container* ptr;
public:
	static void set_true_bit(container& n, int position) {
		n |= degrees[position];
	}
	static void set_false_bit(container& n, int position) {
		n &= ~degrees[position];
	}

	static bool get_bit(container n, int position) {
		return (n >> position) & one;
	}

	BitArray(int size = 0, bool default_value = false) : _size(size) {
		int num_arrays = ((size + for_mod) >> main_degree);
		ptr = new container[num_arrays];

		container default_val = -default_value;
		for (int i = 0; i < num_arrays; ++i) {
			this->ptr[i] = default_val;
		}
	}
	BitArray(const BitArray& other) {
		this->_size = other._size;
		this->ptr = new container[this->_size];
		std::memcpy(this->ptr, other.ptr, sizeof(container) * this->_size);
	}
	BitArray(BitArray&& other) noexcept {
		this->_size = other._size;
		this->ptr = other.ptr;
		other.ptr = nullptr;
	}
	BitArray& operator=(const BitArray& other) = default;
	BitArray& operator=(BitArray&& other) noexcept = default;

	int size() const {
		return this->_size;
	}

	bool operator[](int index) const {
		return get_bit(ptr[(index >> main_degree)], index & for_mod);
	}

	void set_true(int index) {
		set_true_bit(ptr[(index >> main_degree)], index & for_mod);
	}

	void set_false(int index) {
		set_false_bit(ptr[(index >> main_degree)], index & for_mod);
	}

	~BitArray() {
		delete[] this->ptr;
	}

	BitArray operator+(const BitArray& other) const {
		BitArray answer;
		answer._size = this->size() + other.size();
		answer.ptr = new container[answer.size()];
		memcpy(answer.ptr, this->ptr, sizeof(container) * this->size());
		memcpy(answer.ptr + this->size(), other.ptr, sizeof(container) * other.size());
		return answer;
	}
};
