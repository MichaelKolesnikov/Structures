#pragma once
#include "EratosthenesSieve.hpp"

namespace NumberTheory {
	class SegmentedWheel {
	private:
		const int length_of_buffer = 200 * 1024;
		const int wheel = 30;
		const int wheel_primes_count = 3;

		static int wheel_remainders[];
		static int skipped_primes[];
		static unsigned char masks[];
		static std::vector<int> offsets_per_byte[256];
		static bool static_constructed;

		size_t length;
		std::vector<int> first_primes;
		std::vector<std::vector<int>> prime_multiples;

		static void static_constructor() {
			if (SegmentedWheel::static_constructed) {
				return;
			}
			std::vector<int> offsets;
			for (int b = 0; b < 256; ++b) {
				offsets.clear();
				for (int i = 0; i < 8; ++i) {
					if ((b & masks[i]) != 0) {
						offsets.push_back(wheel_remainders[i]);
					}
				}
				offsets_per_byte[b] = offsets;
			}
			SegmentedWheel::static_constructed = true;
		}

		void SieveSegment(unsigned char* segmentData, size_t segmentStart, size_t segmentEnd) {
			for (int i = 0; i < length_of_buffer; ++i) {
				segmentData[i] = 255;
			}
			auto segmentLength = segmentEnd - segmentStart;

			for (int i = 0; i < 8; ++i) {
				unsigned char mask = (unsigned char)~masks[i];
				for (int j = 0; j < prime_multiples[i].size(); ++j) {
					auto current = prime_multiples[i][j] - segmentStart;
					if (current >= segmentLength)
						continue;
					auto prime = first_primes[j];

					while (current < segmentLength) {
						segmentData[current] &= mask;
						current += prime;
					}

					this->prime_multiples[i][j] = segmentStart + current;
				}
			}
		}

	public:
		SegmentedWheel(size_t length) {
			static_constructor();
			this->length = length;
			auto firstChunkLength = (size_t)std::sqrt(length) + 1;
			NumberTheory::EratosthenesSieve sieve(firstChunkLength);
			std::vector<size_t> firstPrimes;
			auto prime_process = [&firstPrimes](const size_t& p) {firstPrimes.push_back(p);};
			sieve.build();
			sieve.go_through_prime_numbers(prime_process);
			if (wheel_primes_count <= firstPrimes.size()) {
				this->first_primes = std::vector<int>(std::begin(firstPrimes) + wheel_primes_count, std::end(firstPrimes));
			}
			prime_multiples.resize(8);
			for (int i = 0; i < 8; ++i) {
				prime_multiples[i].resize(first_primes.size());
				for (int j = 0; j < first_primes.size(); ++j) {
					auto prime = first_primes[j];
					auto val = prime * prime;
					while (val % wheel != wheel_remainders[i]) {
						val += 2 * prime;
					}
					prime_multiples[i][j] = (val - wheel_remainders[i]) / wheel;
				}
			}
		}

		void ListPrimes(const std::function<void(const size_t&)>& callback) {
			for (int i = 0; i < 10; ++i)
				if (skipped_primes[i] < this->length)
					callback(skipped_primes[i]);
			size_t max_ = (this->length + this->wheel - 1) / this->wheel;
			unsigned char* segmentData = new unsigned char[length_of_buffer];
			size_t segmentStart = 1;
			auto segmentEnd = std::min(segmentStart + length_of_buffer, max_);
			while (segmentStart < max_) {
				SieveSegment(segmentData, segmentStart, segmentEnd);
				for (int i = 0; i < length_of_buffer; ++i) {
					auto offset = (segmentStart + i) * this->wheel;
					auto data = segmentData[i];
					auto& current_offsets = SegmentedWheel::offsets_per_byte[data];
					for (int j = 0; j < current_offsets.size(); ++j) {
						auto p = offset + current_offsets[j];
						if (p >= this->length) {
							break;
						}
						callback(p);
					}
				}
				segmentStart = segmentEnd;
				segmentEnd = std::min(segmentStart + this->length_of_buffer, max_);
			}
			delete[] segmentData;
		}
	};

	int SegmentedWheel::wheel_remainders[] = { 1, 7, 11, 13, 17, 19, 23, 29 };
	int SegmentedWheel::skipped_primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
	unsigned char SegmentedWheel::masks[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
	std::vector<int> SegmentedWheel::offsets_per_byte[256];
	bool SegmentedWheel::static_constructed = false;
}
