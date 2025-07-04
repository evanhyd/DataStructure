#pragma once
#include <memory>
#include <mutex>
#include <optional>

namespace flow {

  template <typename T>
  class ConcurrentFlexQueue {

    struct Node {
      std::optional<T> value;
      std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head_;
    Node* tail_;
    mutable std::mutex headMux_{};
    mutable std::mutex tailMux_{};
    std::condition_variable blocked_{};

  public:
    ConcurrentFlexQueue()
    : head_(std::make_unique<Node>()), tail_(head_.get()) {
    }

    ConcurrentFlexQueue(const ConcurrentFlexQueue&) = delete;
    ConcurrentFlexQueue(ConcurrentFlexQueue&&) = delete;
    ConcurrentFlexQueue& operator=(const ConcurrentFlexQueue&) = delete;
    ConcurrentFlexQueue& operator=(ConcurrentFlexQueue&&) = delete;

    /// <summary>
    /// </summary>
    /// <returns>True if the container size is 0.</returns>
    bool empty() const {
      std::scoped_lock lock(headMux_, tailMux_);
      return head_.get() == tail_;
    }

    /// <summary>
    /// Push a new element to the concurrent flex queue.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value">The element to add.</param>
    template <typename U>
    void push(U&& value) {
      std::unique_ptr<Node> dummy = std::make_unique<Node>();
      {
        std::lock_guard lock(tailMux_);
        tail_->value = std::forward<U>(value);
        tail_->next = std::move(dummy);
        tail_ = tail_->next.get();
      }
      blocked_.notify_one();
    }

    /// <summary>
    /// Construct a new element in the concurrent flex queue inplace.
    /// </summary>
    /// <typeparam name="...Args"></typeparam>
    /// <param name="...args">The element constructor's arguments.</param>
    template <typename ...Args>
    void emplace(Args&&... args) {
      std::unique_ptr<Node> dummy = std::make_unique<Node>();
      {
        std::lock_guard lock(tailMux_);
        tail_->value = T{ std::forward<Args>(args)... };
        tail_->next = std::move(dummy);
        tail_ = tail_->next.get();
      }
      blocked_.notify_one();
    }

    /// <summary>
    /// Try return the first element in the concurrent flex queue.
    /// If the concurrent flex queue is empty, then return std::nullopt.
    /// </summary>
    /// <returns>A copy of the first element if not empty, otherwise std::nullopt.</returns>
    std::optional<T> tryFront() const {
      std::unique_lock lock(headMux_);
      if (head_.get() == tail_) {
        return std::nullopt;
      }
      return head_->value;
    }

    /// <summary>
    /// Try pop and return the first element in the concurrent flex queue.
    /// If the concurrent flex queue is empty, then return std::nullopt.
    /// </summary>
    /// <returns>The first moved element if not empty, otherwise std::nullopt.</returns>
    std::optional<T> tryPop() {
      std::unique_lock lock(headMux_);
      if (head_.get() == tail_) {
        return std::nullopt;
      }

      auto oldHead = std::move(head_);
      head_ = std::move(oldHead->next);
      return std::move(oldHead->value);
    }

    /// <summary>
    /// Wait until the concurrent flex queue has at least one element,
    /// then pop and return the first element in the concurrent flex queue.
    /// </summary>
    /// <returns>The first element.</returns>
    T waitPop() {
      std::unique_lock lock(headMux_);
      blocked_.wait(lock, [&]() {
        return head_.get() != tail_;
      });

      auto oldHead = std::move(head_);
      head_ = std::move(oldHead->next);
      return oldHead->value.value();
    }
  };
}
