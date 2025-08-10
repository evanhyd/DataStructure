#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace flow {

  // A work-stealing queue. Similar to the concurrent queue, but it allows trySteal that pop the last element.
  template <typename T>
  class WorkStealingQueue {
    using allocator_type = typename std::deque<T>::allocator_type;

    mutable std::mutex mux_;
    std::condition_variable blocked_;
    std::deque<T> queue_;

  public:
    /// @brief Constructs a concurrent FIFO queue.
    WorkStealingQueue() = default;

    /// @brief Constructs a concurrent FIFO queue.
    explicit WorkStealingQueue(const allocator_type& allocator)
      : queue_(allocator) {
    }

    WorkStealingQueue(const WorkStealingQueue&) = delete;
    WorkStealingQueue(WorkStealingQueue&&) = delete;
    WorkStealingQueue& operator=(const WorkStealingQueue&) = delete;
    WorkStealingQueue& operator=(WorkStealingQueue&&) = delete;

    /// @brief Checks if the queue is empty. Value can be obsolete in concurrency code.
    /// @return True if empty, false otherwise.
    bool empty() const {
      std::lock_guard lock(mux_);
      return queue_.empty();
    }

    /// @brief Returns the number of elements in the queue. Value can be obsolete in concurrency code.
    std::size_t size() const {
      std::lock_guard lock(mux_);
      return queue_.size();
    }

    /// @brief Pushes a new element into the queue.
    void push(const T& value) {
      {
        std::lock_guard lock(mux_);
        queue_.push_back(value);
      }
      blocked_.notify_one();
    }

    /// @brief Pushes a new element into the queue.
    void push(T&& value) {
      {
        std::lock_guard lock(mux_);
        queue_.push_back(std::move(value));
      }
      blocked_.notify_one();
    }

    /// @brief Constructs a new element in place at the end of the queue.
    template <typename... Args>
    void emplace(Args&&... args) {
      {
        std::lock_guard lock(mux_);
        queue_.emplace_back(std::forward<Args>(args)...);
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

    /// @brief Tries to pop front and return the first element.
    /// @return The first element moved or std::nullopt if empty.
    std::optional<T> tryPop() {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }
      std::optional<T> value = std::move(queue_.front());

      queue_.pop_front();
      return value;
    }

    /// @brief Waits until the queue is not empty, then pops and returns the first element.
    /// @return The first element.
    T waitPop() {
      std::unique_lock lock(mux_);
      blocked_.wait(lock, [&]() { return !queue_.empty(); });

      T value = std::move(queue_.front());
      queue_.pop_front();
      return value;
    }

    /// @brief Tries to pop back and return the last element.
    /// @return The last element moved or std::nullopt if empty.
    std::optional<T> trySteal() {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }
      std::optional<T> value = std::move(queue_.back());

      queue_.pop_back();
      return value;
    }
  };

}
