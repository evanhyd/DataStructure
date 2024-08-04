#pragma once
#include <algorithm>
#include <type_traits>
#include <iterator>

template <typename T>
class IntegerIterator {
  T _value;

public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = T; //intentional
  using pointer = T*;
  using iterator_category = std::random_access_iterator_tag;

  IntegerIterator(const T& value) : _value(value) {
    using Trait = typename std::iterator_traits<IntegerIterator>::iterator_category;
    static_assert(std::is_convertible_v<Trait, std::bidirectional_iterator_tag>, "not bidirectional iterator");
    static_assert(std::is_convertible_v<Trait, std::random_access_iterator_tag>, "not random access iterator");
    static_assert(std::is_integral_v<T>, "IntegerIterator only supports integer type");
  }

  reference operator*() const { return _value; }
  pointer operator->() const { return &_value; }

  IntegerIterator& operator++() {
    ++_value;
    return *this;
  }

  IntegerIterator operator++(int) {
    IntegerIterator it = *this;
    ++*this;
    return it;
  }

  IntegerIterator& operator--() {
    --_value;
    return *this;
  }

  IntegerIterator operator--(int) {
    IntegerIterator it = *this;
    --*this;
    return it;
  }

  IntegerIterator& operator+=(difference_type n) {
    _value += n;
    return *this;
  }

  difference_type operator-(IntegerIterator& rhs) const {
    return _value - rhs._value;
  }

  reference operator[](difference_type n) const {
    return _value + n;
  }

  friend bool operator<(const IntegerIterator& lhs, const IntegerIterator& rhs) {
    return lhs._value < rhs._value;
  }

  friend bool operator==(const IntegerIterator& lhs, const IntegerIterator& rhs) {
    return lhs._value == rhs._value;
  }

  friend void swap(IntegerIterator& lhs, IntegerIterator& rhs) {
    using std::swap;
    swap(lhs._value, rhs._value);
  }
};

template <typename T>
IntegerIterator<T>& operator-=(IntegerIterator<T>& lhs, typename IntegerIterator<T>::difference_type n) {
  lhs += -n;
  return lhs;
}

template <typename T>
IntegerIterator<T> operator+(IntegerIterator<T> lhs, typename IntegerIterator<T>::difference_type n) {
  lhs += n;
  return lhs;
}

template <typename T>
IntegerIterator<T> operator+(typename IntegerIterator<T>::difference_type n, IntegerIterator<T> rhs) {
  return rhs + n;
}

template <typename T>
IntegerIterator<T> operator-(IntegerIterator<T> lhs, typename IntegerIterator<T>::difference_type n) {
  lhs -= n;
  return lhs;
}

template <typename T>
bool operator>(const IntegerIterator<T>& lhs, const IntegerIterator<T>& rhs) {
  return rhs < lhs;
}

template <typename T>
bool operator>=(const IntegerIterator<T>& lhs, const IntegerIterator<T>& rhs) {
  return !(lhs < rhs);
}

template <typename T>
bool operator<=(const IntegerIterator<T>& lhs, const IntegerIterator<T>& rhs) {
  return !(lhs > rhs);
}

template <typename T>
bool operator!=(const IntegerIterator<T>& lhs, const IntegerIterator<T>& rhs) {
  return !(lhs == rhs);
}
