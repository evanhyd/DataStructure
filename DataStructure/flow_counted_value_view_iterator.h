#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>

namespace flow {

  /// <summary>
  /// Iterator that returns a constant value for a fixed number of times.
  /// Useful for creating a virtual range of repeated values without storage.
  /// </summary>
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

    CountedValueViewIterator() :
      value_(nullptr), count_(0) {
    }

    CountedValueViewIterator(const T& value, std::size_t count = 0) :
      value_(&value), count_(count) {
    }

    const T& operator*() const {
      assert(value_ && "no value");
      return *value_;
    }

    const T* operator->() const {
      assert(value_ && "no value");
      return value_;
    }

    CountedValueViewIterator& operator++() {
      assert(count_ > 0 && "negative count");
      --count_;
      return *this;
    }

    CountedValueViewIterator operator++(int) {
      CountedValueViewIterator cpy = *this;
      --count_;
      return cpy;
    }

    friend bool operator==(const CountedValueViewIterator& lhs, const CountedValueViewIterator& rhs) {
      return lhs.count_ == rhs.count_ && lhs.value_ == rhs.value_;
    }

    friend bool operator!=(const CountedValueViewIterator& lhs, const CountedValueViewIterator& rhs) {
      return !(lhs == rhs);
    }
  };
}
