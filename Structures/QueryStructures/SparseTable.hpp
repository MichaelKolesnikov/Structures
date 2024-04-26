#pragma once
#include "..//NumberTheory//FloorLog.hpp"
#include <algorithm>
#include <functional>

namespace QueryStructures {
	template <class T>
	using BinaryFunction = std::function<T(const T&, const T&)>;

	template <class T>
	class ISparseTable {
	protected:
		size_t n;
		size_t size, log;

		std::vector<T> a;
		BinaryFunction<T> f;

		virtual void initialize() = 0;

		void constructor(const std::vector<T>& v, const BinaryFunction<T>& func) {
			this->n = v.size();
			this->f = func;
			this->size = this->n;
			this->log = NumberTheory::FloorLog::get_floor_log(this->n) + 1;
			this->a.resize(n);
			std::copy(v.begin(), v.end(), this->a.begin());
			this->initialize();
		}
	public:
		size_t get_size() const {
			return this->size();
		}

		size_t get_count_of_levels() const {
			return this->log;
		}

		virtual T ask_value(size_t l, size_t r) const = 0;
	};

	/// <summary>
	/// The SparseTableWithoutIdempotency class represents a data structure 
	/// for efficiently searching for the result of 
	/// an associative, cumulative, idempotent function on a subsegment of an array.
	/// </summary>
	/// <typeparam name="T">The set T must have order</typeparam>
	/// 
	/// <remarks>
	/// Asymptotics:
	/// - Building the data structure: O(N log N), where N is the size of the input array.
	/// - Query operation (ask): O(1) (after preprocessing).
	/// </remarks>
	template <class T>
	class SparseTableWithIdempotency : public ISparseTable<T> {
	private:
		std::vector<std::vector<int>> sparse;

		size_t get_index(size_t i, size_t j) const {
			return (this->f(this->a[i], this->a[j]) == this->a[i] ? i : j);
		}

		void initialize() override {
			this->sparse.resize(this->log, std::vector<int>(this->size));
			for (int j = 0; j < this->n; ++j) {
				this->sparse[0][j] = j;
			}
			for (int level = 1; level < this->log; ++level) {
				for (int i = 0; i + (1 << level) <= this->n; ++i) {
					this->sparse[level][i] = get_index(this->sparse[level - 1][i], this->sparse[level - 1][i + (1 << level - 1)]);
				}
			}
		}
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		/// <param name="func">function must provide the properties: associativity, commutativity, idempotency</param>
		SparseTableWithIdempotency(const std::vector<T>& v, const BinaryFunction<T>& func) {
			this->constructor(v, func);
		}

		size_t ask_index(size_t l, size_t r) const {
			size_t len = r - l + 1;
			size_t level = NumberTheory::FloorLog::get_floor_log(len);
			auto left = sparse[level][l];
			auto right = sparse[level][r - (static_cast<unsigned long long>(1) << level) + 1];
			return get_index(left, right);
		}

		T ask_value(size_t l, size_t r) const override {
			auto index = this->ask_index(l, r);
			return this->a[index];
		}
	};

	/// <summary>
	/// The SparseTable class represents a data structure 
	/// for efficiently searching for the result of 
	/// an associative, cumulative function on a subsegment of an array.
	/// </summary>
	/// <typeparam name="T">The set T must have order</typeparam>
	/// 
	/// <remarks>
	/// Asymptotics:
	/// - Building the data structure: O(N log N), where N is the size of the input array.
	/// - Query operation (ask): O(1) (after preprocessing).
	/// </remarks>
	template <class T>
	class SparseTable : public ISparseTable<T> {
	private:
		std::vector<std::vector<T>> sparse;

		void initialize() override {
			this->sparse.resize(this->log, std::vector<int>(this->size));
			for (int j = 0; j < this->n; ++j) {
				this->sparse[0][j] = this->a[j];
			}
			for (int level = 1; level < this->log; ++level) {
				for (int i = 0; i + (1 << level) <= this->n; ++i) {
					this->sparse[level][i] = this->f(this->sparse[level - 1][i], this->sparse[level - 1][i + (1 << (level - 1))]);
				}
			}
		}
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		/// <param name="func">function must provide the properties: associativity, commutativity</param>
		SparseTable(const std::vector<T>& v, const BinaryFunction<T>& func) {
			this->constructor(v, func);
		}

		T ask_value(size_t l, size_t r) const override {
			auto result = sparse[0][l];
			++l;
			for (int level = this->log; level >= 0; --level) {
				if (l + (static_cast<unsigned long long>(1) << level) - 1 <= r) {
					result = this->f(result, sparse[level][l]);
					l += (static_cast<unsigned long long>(1) << level);
				}
			}
			return result;
		}
	};
}
