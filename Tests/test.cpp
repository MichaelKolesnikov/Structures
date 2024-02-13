#include "pch.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include "..\\Structures\\NumberTheory\\NumberTheory.h"
#include "..\\Structures\\SparseTable.h"


TEST(FloorLogTest, LogarithmTest) {
	for (size_t i = 1; i <= 1000; ++i) {
		auto computed_log = NumberTheory::FloorLog::get_floor_log(i);
		auto library_log = static_cast<size_t>(std::log2(i));
		ASSERT_EQ(computed_log, library_log) << "Incorrect logarithm for " << i;
	}
}


TEST(FloorLogTest, InitializationTest) {
	NumberTheory::FloorLog::initialize_to_or_clear_after(100);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(1), 0);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(2), 1);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(4), 2);

	NumberTheory::FloorLog::initialize_to_or_clear_after(200);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(1), 0);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(2), 1);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(4), 2);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(128), 7);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(256), 8);

	NumberTheory::FloorLog::initialize_to_or_clear_after(50);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(1), 0);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(2), 1);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(4), 2);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(32), 5);
	ASSERT_EQ(NumberTheory::FloorLog::get_floor_log(64), 6);
}


namespace NumberTheory {
	TEST(GCDLCMTest, GCDTest) {
		ASSERT_EQ(gcd(10, 15), 5);
		ASSERT_EQ(gcd(35, 49), 7);
		ASSERT_EQ(gcd(18, 24), 6);
	}

	TEST(GCDLCMTest, LCMTest) {
		ASSERT_EQ(lcm(10, 15), 30);
		ASSERT_EQ(lcm(35, 49), 245);
		ASSERT_EQ(lcm(18, 24), 72);
	}
}

TEST(GCDLambdaTest, GCDLambdaTest) {
	auto gcd = [](int a, int b) {
		return NumberTheory::get_greatest_common_divisor(a, b);
	};

	for (int i = 0; i < 1e1; ++i) {
		for (int j = 0; j < 1e1; ++j) {
			auto g1 = gcd(i, j);
			auto g2 = gcd(-i, j);
			auto g3 = gcd(i, -j);
			auto g4 = gcd(-i, -j);

			ASSERT_TRUE(g1 == g2 && g2 == g3 && g3 == g4 && g4 == g1);
		}
	}
}


TEST(SparseTableTest, MinFunctionTest) {
	std::vector<int> v = { 1, 3, 2, 5, 4 };
	SparseTableWithIdempotency<int> sparseTable(v, [](int a, int b) { return std::min(a, b); });

	ASSERT_EQ(sparseTable.ask_value(1, 3), 2);
	ASSERT_EQ(sparseTable.ask_value(0, 4), 1);
	ASSERT_EQ(sparseTable.ask_value(2, 4), 2);
}


TEST(SparseTableTest, MaxFunctionTest) {
	std::vector<int> v = { 1, 3, 2, 5, 4 };
	SparseTableWithIdempotency<int> sparseTable(v, [](int a, int b) { return std::max(a, b); });

	ASSERT_EQ(sparseTable.ask_value(1, 3), 5);
	ASSERT_EQ(sparseTable.ask_value(0, 4), 5);
	ASSERT_EQ(sparseTable.ask_value(2, 4), 5);
}


TEST(SparseTableWithIdempotencyTest, GCDFunctionTest) {
	std::vector<int> v = { 12, 18, 24, 36, 48 };
	SparseTable<int> sparseTable(v, [](int a, int b) { return NumberTheory::get_greatest_common_divisor(a, b); });

	ASSERT_EQ(sparseTable.ask_value(1, 3), 6);
	ASSERT_EQ(sparseTable.ask_value(0, 4), 6);
	ASSERT_EQ(sparseTable.ask_value(2, 4), 12);
}


TEST(SparseTableWithIdempotencyTest, LCMFunctionTest) {
	std::vector<int> v = { 12, 18, 24, 36, 48 };
	SparseTable<int> sparseTable(v, [](int a, int b) { return NumberTheory::lcm(a, b); });

	ASSERT_EQ(sparseTable.ask_value(1, 3), 72);
	ASSERT_EQ(sparseTable.ask_value(0, 4), 144);
	ASSERT_EQ(sparseTable.ask_value(2, 4), 144);
}


class TestEratosthenesSieve : public ::testing::Test {
protected:
	void SetUp() override {
		size = 10000000;
		test_size = 100;
		sieve.build(size);
	}

	NumberTheory::EratosthenesSieve<128> sieve;
	int size;
	int test_size;
};


TEST_F(TestEratosthenesSieve, PrimeTest) {
	std::vector<int> test_numbers(test_size);
	std::generate(test_numbers.begin(), test_numbers.end(), [this]() { return rand() % (size - 1) + 1; });

	for (auto test_number : test_numbers) {
		ASSERT_EQ(sieve.is_prime(test_number), sieve.is_prime_sqrt_method(test_number));
	}
}


class TestFactorizer : public ::testing::Test {
protected:
	void SetUp() override {
		size = 100000;
		test_size = 100;
		factorizer.build(size);
	}

	NumberTheory::Factorizer<128> factorizer;
	int size;
	int test_size;
};


TEST_F(TestFactorizer, FactorizationTest) {
	std::vector<int> test_numbers(test_size);
	std::generate(test_numbers.begin(), test_numbers.end(), [this]() { return rand() % (size - 1) + 1; });

	for (auto test_number : test_numbers) {
		auto factors = factorizer.factorize(test_number);
		int product = std::accumulate(factors.begin(), factors.end(), 1, std::multiplies<int>());
		ASSERT_EQ(product, test_number);
	}
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
