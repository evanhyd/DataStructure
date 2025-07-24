#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>

namespace flow {

  /// @brief A thread-safe concurrent queue with flexible push and pop operations.
  template <typename T>
  class ConcurrentFlexQueue {
    struct Node {
      std::optional<T> value;
      std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head_;
    Node* tail_;
    mutable std::mutex headMux_;
    mutable std::mutex tailMux_;
    std::condition_variable blocked_;

  public:
    /// @brief Constructs an empty queue.
    ConcurrentFlexQueue()
      : head_(std::make_unique<Node>()), tail_(head_.get()) {
    }

    ConcurrentFlexQueue(const ConcurrentFlexQueue&) = delete;
    ConcurrentFlexQueue(ConcurrentFlexQueue&&) = delete;
    ConcurrentFlexQueue& operator=(const ConcurrentFlexQueue&) = delete;
    ConcurrentFlexQueue& operator=(ConcurrentFlexQueue&&) = delete;

    /// @brief Checks if the queue is empty.
    /// @return True if the queue is empty, false otherwise.
    bool empty() const {
      std::scoped_lock lock(headMux_, tailMux_);
      return head_.get() == tail_;
    }

    /// @brief Pushes a new element into the queue.
    /// @param value The element to add.
    template <typename U>
    void push(U&& value) {
      auto dummy = std::make_unique<Node>();
      {
        std::lock_guard lock(tailMux_);
        tail_->value = std::forward<U>(value);
        tail_->next = std::move(dummy);
        tail_ = tail_->next.get();
      }
      blocked_.notify_one();
    }

    /// @brief Constructs a new element in place at the end of the queue.
    /// @param args Arguments forwarded to construct the new element.
    template <typename... Args>
    void emplace(Args&&... args) {
      auto dummy = std::make_unique<Node>();
      {
        std::lock_guard lock(tailMux_);
        tail_->value.emplace(std::forward<Args>(args)...);
        tail_->next = std::move(dummy);
        tail_ = tail_->next.get();
      }
      blocked_.notify_one();
    }

    /// @brief Attempts to retrieve the first element without removing it.
    /// @return A copy of the first element if the queue is not empty, std::nullopt otherwise.
    std::optional<T> tryFront() const {
      std::unique_lock lock(headMux_);
      if (head_.get() == tail_) {
        return std::nullopt;
      }
      return head_->value;
    }

    /// @brief Attempts to pop and return the first element.
    /// @return The first element moved if queue is not empty, std::nullopt otherwise.
    std::optional<T> tryPop() {
      std::unique_lock lock(headMux_);
      if (head_.get() == tail_) {
        return std::nullopt;
      }

      auto oldHead = std::move(head_);
      head_ = std::move(oldHead->next);
      return std::move(oldHead->value);
    }

    /// @brief Waits until the queue is not empty, then pops and returns the first element.
    /// @return The first element.
    T waitPop() {
      std::unique_lock lock(headMux_);
      blocked_.wait(lock, [&]() { return head_.get() != tail_; });

      auto oldHead = std::move(head_);
      head_ = std::move(oldHead->next);
      return oldHead->value.value();
    }
  };
}
