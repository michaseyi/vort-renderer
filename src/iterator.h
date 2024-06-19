#pragma once
#include <coroutine>
#include <iterator>

template <typename T>
class Iterator {
 public:
  struct promise_type {
    T current_value;

    auto get_return_object() {
      return std::coroutine_handle<promise_type>::from_promise(*this);
    }

    auto initial_suspend() { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    auto yield_value(T value) {
      current_value = value;
      return std::suspend_always{};
    }
  };

  Iterator(std::coroutine_handle<promise_type> coroutine_handle)
      : coroutine_handle_(coroutine_handle) {}

  auto& operator*() { return coroutine_handle_.promise().current_value; }

  bool operator==(std::default_sentinel_t) const {
    return coroutine_handle_.done();
  }

  Iterator& operator++() {
    if (!coroutine_handle_.done()) {
      coroutine_handle_.resume();
    }
    return *this;
  }

  Iterator& begin() {
    if (!coroutine_handle_.done()) {
      coroutine_handle_.resume();
    }
    return *this;
  }

  std::default_sentinel_t end() { return std::default_sentinel; }

  std::coroutine_handle<promise_type> coroutine_handle_;
};