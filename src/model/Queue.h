#pragma once
#include <deque>
#include <utility>
#include <cstddef>

template <typename T>
class Queue {
public:
    explicit Queue(std::size_t capacity = 1000)
        : _capacity(capacity) {
    }

    bool tryPush(T v) {
        if (_q.size() >= _capacity)
            return false;

        _q.push_back(std::move(v));
        return true;
    }

    bool empty() const { return _q.empty(); }
    std::size_t size() const { return _q.size(); }
    std::size_t capacity() const { return _capacity; }
    bool canPush() const { return _q.size() < _capacity; }
    bool full() const { return _q.size() >= _capacity; }

    T pop() {
        T v = std::move(_q.front());
        _q.pop_front();
        return v;
    }

private:
    std::deque<T> _q;
    std::size_t _capacity;
};