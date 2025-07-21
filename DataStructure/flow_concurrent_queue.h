#pragma once
#include <deque>
#include <mutex>
#include <optional>
#include <queue>

namespace flow {
  template <typename T, typename Container = std::deque<T>>
  class ConcurrentQueue {
    using allocator_type = Container::allocator_type;

    std::queue<T, Container> queue_;
    mutable std::mutex mux_{};
    std::condition_variable blocked_{};

  public:
    /// <summary>
    /// Construct a concurrent queue. The elements follow the FIFO order.
    /// </summary>
    /// <param name="allocator">allocates the elements in the concurrent queue.</param>
    explicit ConcurrentQueue(const allocator_type& allocator = allocator_type())
        : queue_(allocator) {
    }

    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue(ConcurrentQueue&&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(ConcurrentQueue&&) = delete;

    /// <summary>
    /// </summary>
    /// <returns>True if the container size is 0.</returns>
    bool empty() const {
      std::lock_guard lock(mux_);
      return queue_.empty();
    }

    /// <summary>
    /// </summary>
    /// <returns>The number of elements in the concurrent queue.</returns>
    std::size_t size() const {
      std::lock_guard lock(mux_);
      return queue_.size();
    }

    /// <summary>
    /// Push a new element to the concurrent queue.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value">The element to add.</param>
    template <typename U>
    void push(U&& value) {
      {
        std::lock_guard lock(mux_);
        queue_.push(std::forward<U>(value));
      }
      blocked_.notify_one();
    }

    /// <summary>
    /// Construct a new element in the concurrent queue inplace.
    /// </summary>
    /// <typeparam name="...Args"></typeparam>
    /// <param name="...args">The element constructor's arguments.</param>
    template <typename ...Args>
    void emplace(Args&&... args) {
      {
        std::lock_guard lock(mux_);
        queue_.emplace(std::forward<Args>(args)...);
      }
      blocked_.notify_one();
    }

    /// <summary>
    /// Try return the first element in the concurrent queue.
    /// If the concurrent queue is empty, then return std::nullopt.
    /// </summary>
    /// <returns>A copy of the first element if not empty, otherwise std::nullopt.</returns>
    std::optional<T> tryFront() const {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }
      return queue_.front();
    }

    /// <summary>
    /// Try pop and return the first element in the concurrent queue.
    /// If the concurrent queue is empty, then return std::nullopt.
    /// </summary>
    /// <returns>The first moved element if not empty, otherwise std::nullopt.</returns>
    std::optional<T> tryPop() {
      std::lock_guard lock(mux_);
      if (queue_.empty()) {
        return std::nullopt;
      }

      std::optional<T> value{ std::move(queue_.front()) };
      queue_.pop();
      return value;
    }

    /// <summary>
    /// Wait until the concurrent queue has at least one element,
    /// then pop and return the first element in the concurrent queue.
    /// </summary>
    /// <returns>The first element.</returns>
    T waitPop() {
      std::unique_lock lock(mux_);
      blocked_.wait(lock, [&]() {
        return !queue_.empty();
      });

      T value = std::move(queue_.front());
      queue_.pop();
      return value;
    }
  };
}
