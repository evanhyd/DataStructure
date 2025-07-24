#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <queue>

namespace flow {
  template <typename T, typename Container = std::deque<T>>
  class ConcurrentQueue {
    using allocator_type = typename Container::allocator_type;

    std::queue<T, Container> queue_;
    mutable std::mutex mux_;
    std::condition_variable blocked_;

  public:
    /// @brief Constructs a concurrent FIFO queue.
    explicit ConcurrentQueue(const allocator_type& allocator = allocator_type())
      : queue_(allocator) {
    }

    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue(ConcurrentQueue&&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(ConcurrentQueue&&) = delete;

    /// @brief Checks if the queue is empty.
    /// @return True if empty, false otherwise.
    bool empty() const {
      std::lock_guard lock(mux_);
      return queue_.empty();
    }

    /// @brief Returns the number of elements in the queue.
    std::size_t size() const {
      std::lock_guard lock(mux_);
      return queue_.size();
    }

    /// @brief Pushes a new element into the queue.
    template <typename U>
    void push(U&& value) {
      {
        std::lock_guard lock(mux_);
        queue_.push(std::forward<U>(value));
      }
      blocked_.notify_one();
    }

    /// @brief Constructs a new element in place at the end of the queue.
    template <typename... Args>
    void emplace(Args&&... args) {
      {
        std::lock_guard lock(mux_);
        queue_.emplace(std::forward<Args>(args)...);
      }
      blocked_.notify_one();
    }

    /// @brief Tries to get a copy of the first element without removing it.
    /// @return A copy of the first element or std::nullopt if empty.
    std::optional<T> tryFront() const {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }
      return queue_.front();
    }

    /// @brief Tries to pop and return the first element.
    /// @return The first element moved or std::nullopt if empty.
    std::optional<T> tryPop() {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }
      std::optional<T> value{ std::move(queue_.front()) };
      queue_.pop();
      return value;
    }

    /// @brief Waits until the queue is not empty, then pops and returns the first element.
    /// @return The first element.
    T waitPop() {
      std::unique_lock lock(mux_);
      blocked_.wait(lock, [&]() { return !queue_.empty(); });

      T value = std::move(queue_.front());
      queue_.pop();
      return value;
    }
  };
}
