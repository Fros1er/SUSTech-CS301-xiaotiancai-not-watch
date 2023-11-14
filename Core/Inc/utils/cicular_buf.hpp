#pragma once

#include <cstddef>
template <typename T, size_t SIZE>
class CicularBuf {
    T buf_[SIZE];
    size_t now_{0}, len_{0};

   public:
    T &operator[](size_t idx) {
        return buf_[idx];
    }

    void push(const T &t) {
        len_ = (len_ + 1) % SIZE;
        buf_[len_] = t;
    }

    T &now() {
        return buf_[now_];
    }

    void pop() {
        if (!is_empty()) {
            now_ = (now_ + 1) % SIZE;
        }
    }

    bool is_empty() const {
        return now_ == len_;
    }
};
