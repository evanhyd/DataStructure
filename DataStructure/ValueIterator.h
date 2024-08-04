#pragma once
#include <algorithm>
#include <type_traits>
#include <iterator>

template <typename T>
class ValueIterator {
  T _value;

public:
  using value_type = T;
  using difference_type = T;
  using reference = T; //intentional
  using pointer = T*;
  using iterator_category = std::random_access_iterator_tag;

  ValueIterator(const T& value) : _value(value) {
    using Trait = typename std::iterator_traits<ValueIterator>::iterator_category;
    static_assert(std::is_convertible_v<Trait, std::bidirectional_iterator_tag>, "not bidirectional iterator");
    static_assert(std::is_convertible_v<Trait, std::random_access_iterator_tag>, "not random access iterator");
    static_assert(std::is_integral_v<T>, "ValueIterator currently only supports integer (float has issues with STL binary search implementation)");
  }

  reference operator*() const { return _value; }
  pointer operator->() const { return &_value; }

  ValueIterator& operator++() {
    ++_value;
    return *this;
  }

  ValueIterator operator++(int) {
    ValueIterator it = *this;
    ++*this;
    return it;
  }

  ValueIterator& operator--() {
    --_value;
    return *this;
  }

  ValueIterator operator--(int) {
    ValueIterator it = *this;
    --*this;
    return it;
  }

  ValueIterator& operator+=(difference_type n) {
    _value += n;
    return *this;
  }

  difference_type operator-(ValueIterator& rhs) const {
    return _value - rhs._value;
  }

  reference operator[](difference_type n) const {
    return _value + n;
  }

  friend bool operator<(const ValueIterator& lhs, const ValueIterator& rhs) {
    return lhs._value < rhs._value;
  }

  friend bool operator==(const ValueIterator& lhs, const ValueIterator& rhs) {
    return lhs._value == rhs._value;
  }

  friend void swap(ValueIterator& lhs, ValueIterator& rhs) {
    using std::swap;
    swap(lhs._value, rhs._value);
  }
};

template <typename T>
ValueIterator<T>& operator-=(ValueIterator<T>& lhs, typename ValueIterator<T>::difference_type n) {
  lhs += -n;
  return lhs;
}

template <typename T>
ValueIterator<T> operator+(ValueIterator<T> lhs, typename ValueIterator<T>::difference_type n) {
  lhs += n;
  return lhs;
}

template <typename T>
ValueIterator<T> operator+(typename ValueIterator<T>::difference_type n, ValueIterator<T> rhs) {
  return rhs + n;
}

template <typename T>
ValueIterator<T> operator-(ValueIterator<T> lhs, typename ValueIterator<T>::difference_type n) {
  lhs -= n;
  return lhs;
}

template <typename T>
bool operator>(const ValueIterator<T>& lhs, const ValueIterator<T>& rhs) {
  return rhs < lhs;
}

template <typename T>
bool operator>=(const ValueIterator<T>& lhs, const ValueIterator<T>& rhs) {
  return !(lhs < rhs);
}

template <typename T>
bool operator<=(const ValueIterator<T>& lhs, const ValueIterator<T>& rhs) {
  return !(lhs > rhs);
}

template <typename T>
bool operator!=(const ValueIterator<T>& lhs, const ValueIterator<T>& rhs) {
  return !(lhs == rhs);
}
