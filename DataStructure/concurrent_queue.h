#pragma once
#include <condition_variable>
#include <optional>
#include <queue>
#include <shared_mutex>

namespace flow {
  template <typename T, typename Container = std::deque<T>>
  class ConcurrentQueue {
    std::queue<T, Container> queue_;
    std::shared_mutex rwMutex_;
    std::condition_variable_any blocked_;

  public:
    bool empty() const {
      std::shared_lock lock(rwMutex_);
      return queue_.empty();
    }

    size_t size() const {
      std::shared_lock lock(rwMutex_);
      return queue_.size();
    }

    // Return a copy of the first element in queue.
    T front() const {
      std::shared_lock lock(rwMutex_);
      return queue_.front();
    }

    template <typename ...Args>
    void push(Args&&... args) {
      std::lock_guard lock(rwMutex_);
      queue_.push(std::forward<Args>(args)...);
      blocked_.notify_one();
    }

    T waitAndPop() {
      std::unique_lock lock(rwMutex_);
      blocked_.wait(lock, [this]() {
        return !this->queue_.empty();
      });

      T value = queue_.front();
      queue_.pop();
      return value;
    }

    std::optional<T> tryPop() {
      std::lock_guard lock(rwMutex_);
      if (queue_.empty()) {
        return std::nullopt;
      }

      T value = queue_.front();
      queue_.pop();
      return value;
    }
  };
}
