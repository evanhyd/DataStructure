#pragma once
#include <concepts>
#include <iterator>

namespace flow {

  /// @brief An iterator that can iterate through a range of integer.
  /// Useful when the search space is too big to fit into a container.
  template <std::integral T>
  class IntegralIterator {
    T value_;

  public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T;  // yields by value
    using pointer = void; // no real pointer
    using iterator_category = std::random_access_iterator_tag;

    explicit constexpr IntegralIterator(T value) noexcept : value_(value) {}

    constexpr reference operator*() const noexcept {
      return value_;
    }

    constexpr IntegralIterator& operator++() noexcept {
      ++value_;
      return *this;
    }

    constexpr IntegralIterator operator++(int) noexcept {
      IntegralIterator tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr IntegralIterator& operator--() noexcept {
      --value_;
      return *this;
    }

    constexpr IntegralIterator operator--(int) noexcept {
      IntegralIterator tmp = *this;
      --*this;
      return tmp;
    }

    constexpr IntegralIterator& operator+=(difference_type n) noexcept {
      value_ += static_cast<T>(n);
      return *this;
    }

    constexpr IntegralIterator& operator-=(difference_type n) noexcept {
      value_ -= static_cast<T>(n);
      return *this;
    }

    constexpr reference operator[](difference_type n) const noexcept { 
      return value_ + static_cast<reference>(n);
    }

    constexpr difference_type operator-(const IntegralIterator& rhs) const noexcept {
      return static_cast<difference_type>(value_) - static_cast<difference_type>(rhs.value_);
    }

    constexpr auto operator<=>(const IntegralIterator&) const = default;

    friend void swap(IntegralIterator& lhs, IntegralIterator& rhs) noexcept {
      using std::swap;
      swap(lhs.value_, rhs.value_);
    }
  };

  template <std::integral T>
  constexpr IntegralIterator<T> operator+(IntegralIterator<T> it,
                                          typename IntegralIterator<T>::difference_type n) noexcept {
    it += n;
    return it;
  }

  template <std::integral T>
  constexpr IntegralIterator<T> operator+(typename IntegralIterator<T>::difference_type n,
                                          IntegralIterator<T> it) noexcept {
    return it + n;
  }

  template <std::integral T>
  constexpr IntegralIterator<T> operator-(IntegralIterator<T> it,
                                          typename IntegralIterator<T>::difference_type n) noexcept {
    it -= n;
    return it;
  }
}
