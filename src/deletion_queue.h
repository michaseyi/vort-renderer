#pragma once
#include <deque>
#include <functional>

class DeletionQueue {
 public:
  void push(std::function<void()> deletion_function) {
    deletion_queue_.push_back(deletion_function);
  }
  void flush() {
    for (auto& deletion_function : deletion_queue_) {
      deletion_function();
    }
    deletion_queue_.clear();
  }

 private:
  std::deque<std::function<void()>> deletion_queue_;
};