#pragma once
#include <vector>
#include <bitset>
#include <exception>

template <size_t size_of_one_bitset>
class BitArray {
private:
    size_t _size;
    std::vector<std::bitset<size_of_one_bitset>> vector_of_bitset;

public:
    BitArray(size_t size = 0, bool default_value = false) : _size(size) {
        size_t num_arrays = (size + size_of_one_bitset - 1) / size_of_one_bitset;
        vector_of_bitset = std::vector<std::bitset<size_of_one_bitset>>(num_arrays);
        if (default_value) {
            for (auto& b : this->vector_of_bitset) {
                b.set();
            }
        }
    }

    size_t size() const {
        return this->_size;
    }

    bool operator[](size_t index) const {
        if (index >= this->size()) {
            throw std::out_of_range("Index out of range");
        }

        size_t array_index = index / size_of_one_bitset;
        size_t bit_index = index % size_of_one_bitset;
        return vector_of_bitset[array_index][bit_index];
    }

    typename std::bitset<size_of_one_bitset>::reference operator[](size_t index) {
        if (index >= this->size()) {
            throw std::out_of_range("Index out of range");
        }

        size_t array_index = index / size_of_one_bitset;
        size_t bit_index = index % size_of_one_bitset;
        return vector_of_bitset[array_index][bit_index];
    }
};

