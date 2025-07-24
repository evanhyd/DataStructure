#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>

namespace flow {

  /// @brief Iterator that returns a constant value a fixed number of times.
  /// Useful for creating a virtual range of repeated values without overhead.
  template <typename T>
  class CountedValueViewIterator {
    const T* value_;
    std::size_t count_;

  public:
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    CountedValueViewIterator() noexcept
      : value_(nullptr), count_(0) {
    }

    CountedValueViewIterator(const T& value, std::size_t count = 0) noexcept
      : value_(&value), count_(count) {
    }

    reference operator*() const {
      assert(value_ && "no value");
      return *value_;
    }

    pointer operator->() const {
      assert(value_ && "no value");
      return value_;
    }

    CountedValueViewIterator& operator++() {
      assert(count_ > 0 && "negative count");
      --count_;
      return *this;
    }

    CountedValueViewIterator operator++(int) {
      CountedValueViewIterator tmp = *this;
      --count_;
      return tmp;
    }

    friend bool operator==(const CountedValueViewIterator& lhs, const CountedValueViewIterator& rhs) {
      return lhs.count_ == rhs.count_ && lhs.value_ == rhs.value_;
    }

    friend bool operator!=(const CountedValueViewIterator& lhs, const CountedValueViewIterator& rhs) {
      return !(lhs == rhs);
    }
  };
}
