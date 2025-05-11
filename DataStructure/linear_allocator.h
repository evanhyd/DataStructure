#pragma once
#include <cstddef>
#include <memory>

namespace flow {
  template <typename T>
  class LinearAllocator {
    struct State {
      T* begin_;
      T* current_;
      T* end_;

      explicit constexpr State(std::size_t count)
        : begin_(static_cast<T*>(operator new(count * sizeof(T)))), current_(begin_), end_(begin_ + count) {
      }

      ~State() {
        operator delete(begin_);
      }
    };

    std::shared_ptr<State> state_;

  public:
    using value_type = T;

    explicit constexpr LinearAllocator(std::size_t count = 0)
      : state_(std::make_shared<State>(count)) {
    }

    T* allocate(std::size_t count) const {
      T* block = state_->current_;
      state_->current_ += count;
      if (state_->end_ < state_->current_) {
        throw std::bad_alloc();
      }
      return block;
    }

    void deallocate(T*, std::size_t) const noexcept {
      // Linear allocator does not support deallocation.
    }

    template <typename U, typename ...Args>
    void construct(U* ptr, Args&&... args) const {
      new(ptr) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* ptr) const noexcept {
      ptr->~U();
    }

    friend bool operator==(const LinearAllocator& lhs, const LinearAllocator& rhs) {
      // This equality operator is a bit dubious, when would a linear allocator have the same begin_ but different current_ or end_?
      return lhs.state_.begin_ == rhs.state_.begin_ && lhs.state_.current_ == rhs.state_.current_ && lhs.state_.end_ == rhs.state_.end_;
    }

    friend bool operator!=(const LinearAllocator& lhs, const LinearAllocator& rhs) {
      return !(lhs == rhs);
    }
  };
}
