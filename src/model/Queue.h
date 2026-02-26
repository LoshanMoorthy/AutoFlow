#pragma once
#include <deque>
#include <utility>

template <typename T>
class Queue {
public:
	void push(T v) { _q.push_back(std::move(v)); }
	bool empty() const { return _q.empty(); }
	std::size_t size() const { return _q.size(); }

	T pop() {
		T v = std::move(_q.front());
		_q.pop_front();
		return v;
	}

private:
	std::deque<T> _q;
};