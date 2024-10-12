#pragma once
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

#include "Allocator.h"

namespace flow {
  template <typename T, template<typename> typename Allocator = BasicAllocator>
  class Vector {
  public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

  private:
    Allocator<T> allocator_;
    T* buf_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t capacity) {
      assert(capacity != 0 && "requested 0 capacity");
      
      //copy to the new buffer
      //move constructor should not throw
      T* buf = allocator_.allocate(capacity);
      for (std::size_t i = 0; i < size_; ++i) {
        allocator_.construct(&buf[i], std::move(buf_[i]));
      }

      delete_all();
      buf_ = buf;
      capacity_ = capacity;
    }

    void destroy_range(iterator first, iterator last) noexcept {
      assert(begin() <= first && first <= end() && "first interator out of bound");
      assert(begin() <= last && last <= end() && "last iterator out of bound");
      assert(first <= last && "first iterator denotes a position after last iterator");
      for (; first < last; ++first) {
        allocator_.destroy(first);
      }
    }

    void delete_all() noexcept {
      destroy_range(begin(), end());
      allocator_.deallocate(buf_, capacity_);
    }

  public:
    constexpr Vector() = default;

    constexpr Vector(const Vector<T>& rhs)
      : Vector(rhs.begin(), rhs.end()) {
    }

    constexpr Vector(Vector<T>&& rhs) noexcept
      : allocator_(std::exchange(rhs.allocator_, Allocator<T>())),
        buf_(std::exchange(rhs.buf_, nullptr)), size_(std::exchange(rhs.size_, 0)), capacity_(std::exchange(rhs.capacity_, 0)) {
    }

    explicit constexpr Vector(std::size_t count, const T& value = T{})
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(count)), size_(count), capacity_(count) {
      for (T* first = buf_, *last = buf_ + size_; first != last; ++first) {
        allocator_.construct(first, value);
      }
    }

    constexpr Vector(std::initializer_list<T> list)
      : Vector(list.begin(), list.end()) {
    }

    template <
      typename Iterator,
      std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    explicit constexpr Vector(Iterator first, Iterator last)
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(last - first)), size_(last - first), capacity_(last - first) {
      for (T* dst = buf_; first != last; ++dst, ++first) {
        allocator_.construct(dst, *first);
      }
    }

    template <
      typename Iterator,
      std::enable_if_t<
        !std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category> &&
        std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    explicit Vector(Iterator first, Iterator last)
      : allocator_(Allocator<T>()), buf_(nullptr), size_(0), capacity_(0) {
      for (; first != last; ++first) {
        push_back(*first);
      }
    }

    ~Vector() {
      delete_all();
    }

    Vector& operator=(Vector rhs) noexcept {
      swap(*this, rhs);
      return *this;
    }

    T& operator[](std::size_t i) {
      assert(i < size_ && "index out of bound");
      return buf_[i];
    }

    const T& operator[](std::size_t i) const {
      assert(i < size_ && "index out of bound");
      return buf_[i];
    }

    iterator begin() {
      return buf_;
    }

    constexpr const_iterator begin() const {
      return buf_;
    }

    iterator end() {
      return buf_ + size_;
    }

    constexpr const_iterator end() const {
      return buf_ + size_;
    }

    constexpr std::size_t size() const {
      return size_;
    }

    constexpr std::size_t capacity() const {
      return capacity_;
    }

    constexpr bool empty() const {
      return size_ == 0;
    }

    void clear() const {
      destroy_range(begin(), end());
      size_ = 0;
    }

    T& front() {
      assert(size_ != 0 && "access empty Vector front");
      return buf_[0];
    }

    constexpr const T& front() const {
      assert(size_ != 0 && "access empty Vector front");
      return buf_[0];
    }

    T& back() {
      assert(size_ != 0 && "access empty Vector back");
      return buf_[size_ - 1];
    }

    constexpr const T& back() const {
      assert(size_ != 0 && "access empty Vector back");
      return buf_[size_ - 1];
    }

    void reserve(std::size_t capacity) {
      if (capacity_ < capacity) {
        reallocate(capacity);
      }
    }

    void resize(std::size_t size) {
      if (size_ < size) {
        //expand
        if (capacity_ < size) {
          reallocate(size);
        }
        for (T* first = buf_ + size_, *last = buf_ + size; first != last; ++first) {
          allocator_.construct(first, T{});
        }
      } else {
        //shrink
        destroy_range(buf_ + size, buf_ + size_);
      }
      size_ = size;
    }

    template <typename ...Args>
    void push_back(Args&&... args) {
      if (size_ == capacity_) {
        //consider using a growing ratio < 2, so the sum of previous deallocated memory is greater than the next allocated memory
        reallocate(capacity_ * 2 + 1);
      }
      allocator_.construct(&buf_[size_], std::forward<Args>(args)...);
      ++size_;
    }

    void pop_back() noexcept {
      assert(size_ == 0 && "can not pop back from an empty vector");
      allocator_.destroy(&buf_[size_]);
      --size_;
    }

    void erase(iterator first, iterator last) {
      //replace the erased elements then destroy the rest
      const iterator endIt = end();
      destroy_range(std::move(last, endIt, first), endIt);
      size_ -= last - first;
    }

    template <
      typename Iterator,
      std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    void insert(iterator dst, Iterator first, Iterator last) {
      assert(begin() <= dst && dst <= end() && "destination iterator out of bound");
      assert(first <= last && "first iterator denotes a position after last iterator");

      const std::ptrdiff_t count = last - first;
      const std::size_t available = capacity_ - size_;
      if (available >= count) {
        T* from = buf_ + size_ - 1;
        T* to = from + count;
        for (T* last = from; to != last; --from, --to) {
          allocator_.construct(to, std::move(*from));
        }
        /*for (T* last = dst + count - 1; last < to; --to, --from) {
          *to = std::move(*from);
          *from = 
        }*/
      } else {

      }
    }

    template <
      typename Iterator,
      std::enable_if_t<
      !std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>&&
      std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    void insert(iterator dst, Iterator first, Iterator last) {
      assert(begin() <= dst && dst <= end() && "destination iterator out of bound");
      static_assert(false, "not implemented");
    }

    template<typename MappingFn, typename MappedType = std::invoke_result_t<MappingFn, const T&>>
    Vector<MappedType, Allocator> map(MappingFn fn) const {
      Vector<MappedType, Allocator> mapped;
      mapped.reallocate(size_);
      for (const T& val : *this) {
        mapped.push_back(fn(val));
      }
      return mapped;
    }

    template <typename FilterFn>
    Vector filter(FilterFn fn) const {
      static_assert(std::is_base_of_v<std::invoke_result_t<FilterFn, const T&>, bool>, "filter function must evaluate to bool");
      Vector filtered;
      for (const T& val : *this) {
        if (fn(val)) {
          filtered.push_back(val);
        }
      }
      return filtered;
    }

    friend void swap(Vector& lhs, Vector& rhs) {
      using std::swap;
      swap(lhs.allocator_, rhs.allocator_);
      swap(lhs.buf_, rhs.buf_);
      swap(lhs.size_, rhs.size_);
      swap(lhs.capacity_, rhs.capacity_);
    }

    friend class Vector;
  };
}

template <typename T, template<typename> typename A, template<typename> typename B>
bool operator==(const flow::Vector<T, A>& lhs, const flow::Vector<T, B>& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  typename flow::Vector<T, A>::iterator left = lhs.begin();
  typename flow::Vector<T, A>::iterator end = lhs.end();
  typename flow::Vector<T, B>::iterator right = rhs.begin();
  for (; left != end; ++left, ++right) {
    if (*left != *right) {
      return false;
    }
  }
  return true;
}

template <typename T, template<typename> typename A, template<typename> typename B>
bool operator!=(const flow::Vector<T, A>& lhs, const flow::Vector<T, B>& rhs) {
  return !(lhs == rhs);
}
