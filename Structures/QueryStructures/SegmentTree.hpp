#pragma once
#include <vector>
#include <algorithm>
#include <functional>

namespace QueryStructures {
	template <class T>
	class SegmentTree {
	private:
		size_t n;
		std::vector<T> a;
		std::vector<T> t;
		std::function<T(T, T)> f;

		struct Children {
			size_t left;
			size_t right;

			Children(size_t left, size_t right) : left(left), right(right) {}
			Children(size_t vertex_number) : Children(2 * vertex_number + 1, 2 * vertex_number + 2) {}
		};

		void build(size_t vertex_number, size_t l, size_t r) {
			if (l == r - 1) {
				this->t[vertex_number] = this->a[l];
				return;
			}
			auto m = (l + r) / 2;
			Children children(vertex_number);
			build(children.left, l, m);
			build(children.right, m, r);
			this->t[vertex_number] = this->f(this->t[children.left], this->t[children.right]);
		}

		size_t ask(size_t vertex_number, size_t l, size_t r, size_t askl, size_t askr) { // r & askr are not included
			if (r <= askl || askr <= l) {
				return 0; // vertex is red
			}
			if (askl <= l && r <= askr) {
				return t[vertex_number]; // vertex is green
			}
			Children children(vertex_number);
			auto m = (l + r) / 2;
			return f(ask(children.left, l, m, askl, askr), ask(children.right, m, r, askl, askr)); // vertex is yellow
		}

		void alter(size_t vertex_number, size_t l, size_t r, size_t position, const T& value) {
			if (l == r - 1) {
				this->t[vertex_number] = value;
				return;
			}
			Children children(vertex_number);
			size_t m = (l + r) / 2;
			if (position < m) {
				alter(children.left, l, m, position, value);
			}
			else {
				alter(children.right, m, r, position, value);
			}
			this->t[vertex_number] = this->f(this->t[children.left], this->t[children.right]);
		}

	public:
		SegmentTree(const std::vector<T>& a, const std::function<T(T, T)>& f) : n(a.size()), a(a), f(f) {
			this->t.resize(4 * this->n);
			this->build(0, 0, n);
		}

		T ask_value_on(size_t left, size_t right) {
			return ask(0, 0, this->n, left, right + 1);
		}

		void change_value(size_t position, const T& value) {
			alter(0, 0, this->n, position, value);
		}
	};
}
