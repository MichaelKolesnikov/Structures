#include <iostream>
#include <vector>
#include <algorithm>
#include "NumberTheory//EratosthenesSieve.h"

class OptimizedSegmentedWheel235 {
private:
	const size_t length_of_buffer = 200 * 1024;
	const size_t wheel = 30;
	const size_t wheel_primes_count = 3;
	static std::vector<int> wheel_remainders;
	static std::vector<int> skipped_primes;
	static std::vector<int> masks;
	static std::vector<std::vector<int>> OffsetsPerByte;

	size_t Length;
	std::vector<size_t> FirstPrimes;
	std::vector<std::vector<size_t>> PrimeMultiples;

	static void static_constructor() {
		std::vector<int> offsets;
		for (int b = 0; b < 256; b++) {
			offsets.clear();
			for (int i = 0; i < wheel_remainders.size(); i++) {
				if ((b & masks[i]) != 0) {
					offsets.push_back(wheel_remainders[i]);
				}
			}
			OffsetsPerByte[b] = offsets;
		}
	}

	void SieveSegment(std::vector<int>& segmentData, size_t segmentStart, size_t segmentEnd) {
		for (int i = 0; i < segmentData.size(); i++) {
			segmentData[i] = 255;
		}
		auto segmentLength = segmentEnd - segmentStart;

		for (int i = 0; i < wheel_remainders.size(); i++) {
			int mask = (int)~masks[i];
			for (int j = 0; j < PrimeMultiples[i].size(); j++) {
				auto current = PrimeMultiples[i][j] - segmentStart;
				if (current >= segmentLength) 
					continue;
				auto prime = FirstPrimes[j];

				while (current < segmentLength) {
					segmentData[current] &= mask;
					current += prime;
				}

				this->PrimeMultiples[i][j] = segmentStart + current;
			}
		}
	}

public:
	OptimizedSegmentedWheel235(size_t length) {
		static_constructor();
		this->Length = length;
		auto firstChunkLength = (size_t)std::sqrt(length) + 1;
		NumberTheory::EratosthenesSieve<8> sieve;
		std::vector<size_t> firstPrimes;
		auto prime_process = [&firstPrimes](const size_t& p) {firstPrimes.push_back(p);};
		sieve.build(firstChunkLength, prime_process);
		if (wheel_primes_count <= firstPrimes.size()) {
			this->FirstPrimes = std::vector<size_t>(std::begin(firstPrimes) + wheel_primes_count, std::end(firstPrimes));
		}
		PrimeMultiples.resize(this->wheel_remainders.size());
		for (int i = 0; i < wheel_remainders.size(); i++) {
			PrimeMultiples[i].resize(FirstPrimes.size());
			for (int j = 0; j < FirstPrimes.size(); j++) {
				auto prime = FirstPrimes[j];
				auto val = prime * prime;
				while (val % wheel != wheel_remainders[i]) {
					val += 2 * prime;
				}
				PrimeMultiples[i][j] = (val - wheel_remainders[i]) / wheel;
			}
		}
	}

	void ListPrimes(const std::function<void(const size_t&)>& callback) {
		for (auto prime : skipped_primes)
			if (prime < this->Length)
				callback(prime);
		size_t max_ = (this->Length + this->wheel - 1) / this->wheel;
		std::vector<int> segmentData(length_of_buffer);
		size_t segmentStart = 1;
		auto segmentEnd = std::min(segmentStart + length_of_buffer, max_);
		while (segmentStart < max_) {
			SieveSegment(segmentData, segmentStart, segmentEnd);
			for (size_t i = 0; i < segmentData.size(); ++i) {
				auto offset = (segmentStart + i) * this->wheel;
				auto data = segmentData[i];
				auto current_offsets = OptimizedSegmentedWheel235::OffsetsPerByte[data];
				for (int j = 0; j < current_offsets.size(); j++) {
					auto p = offset + current_offsets[j];
					if (p >= this->Length) {
						break;
					}
					callback(p);
				}
			}
			segmentStart = segmentEnd;
			segmentEnd = std::min(segmentStart + this->length_of_buffer, max_);
		}
	}
};

std::vector<int> OptimizedSegmentedWheel235::wheel_remainders = { 1, 7, 11, 13, 17, 19, 23, 29 };
std::vector<int> OptimizedSegmentedWheel235::skipped_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
std::vector<int> OptimizedSegmentedWheel235::masks = { 1, 2, 4, 8, 16, 32, 64, 128 };
std::vector<std::vector<int>> OptimizedSegmentedWheel235::OffsetsPerByte = std::vector<std::vector<int>>(256);


int main() {
	int n;
	int count = 0;
	std::cin >> n;
	auto f = [&count](const size_t& p) { ++count;};
	OptimizedSegmentedWheel235 sieve(n + 1);
	sieve.ListPrimes(f);
	std::cout << count << std::endl;
}