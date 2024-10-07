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

      //destroy and deallocate the old buffer
      deleteAll();

      buf_ = buf;
      capacity_ = capacity;
    }

    void destroyAll() noexcept {
      for (std::size_t i = 0; i < size_; ++i) {
        allocator_.destroy(&buf_[i]);
      }
    }

    void deleteAll() noexcept {
      destroyAll();
      allocator_.deallocate(buf_, capacity_);
    }

  public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

    constexpr Vector()
      : allocator_(Allocator<T>()), buf_(nullptr), size_(0), capacity_(0) {
    }

    constexpr Vector(const Vector<T>& rhs)
      : Vector(rhs.begin(), rhs.end()) {
    }

    constexpr Vector(Vector<T>&& rhs) noexcept
      : allocator_(std::exchange(rhs.allocator_, Allocator<T>())),
        buf_(std::exchange(rhs.buf_, nullptr)), size_(std::exchange(rhs.size_, 0)), capacity_(std::exchange(rhs.capacity_, 0)) {
    }

    explicit constexpr Vector(std::size_t count, const T& value = T{})
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(count)), size_(count), capacity_(count) {
      for (std::size_t i = 0; i < size_; ++i) {
        allocator_.construct(&buf_[i], value);
      }
    }

    constexpr Vector(std::initializer_list<T> list)
      : Vector(list.begin(), list.end()) {
    }

    template <
      typename Iterator,
      std::enable_if_t<std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    explicit constexpr Vector(Iterator begin, Iterator end)
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(end - begin)), size_(end - begin), capacity_(end - begin) {
      for (T* ptr = buf_; begin != end; ++ptr, ++begin) {
        allocator_.construct(ptr, *begin);
      }
    }

    template <
      typename Iterator,
      std::enable_if_t<
        !std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category> && 
        std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>, int> = 0
    >
    explicit Vector(Iterator begin, Iterator end)
      : allocator_(Allocator<T>()), buf_(nullptr), size_(0), capacity_(0) {
      for (; begin != end; ++begin) {
        push_back(*begin);
      }
    }

    ~Vector() {
      deleteAll();
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
      return iterator{ buf_ };
    }

    constexpr const_iterator begin() const {
      return const_iterator{ buf_ };
    }

    iterator end() {
      return iterator{ buf_ + size_ };
    }

    constexpr const_iterator end() const {
      return const_iterator{ buf_ + size_ };
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
      destroyAll();
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
        for (std::size_t i = size_; i < size; ++i) {
          allocator_.construct(&buf_[i], T{});
        }
      } else {
        //shrink
        for (std::size_t i = size; i < size_; ++i) {
          allocator_.destroy(&buf_[i]);
        }
      }
      size_ = size;
    }

    template <typename ...Args>
    void push_back(Args&&... args) {
      if (size_ == capacity_) {
        //consider using a growing ratio < 2, so the sum of previous deallocated memory is greater than the next allocated memory
        reallocate(capacity_ * 2 + 1);
      }
      allocator_.construct(&buf_[size_], std::forward<Args&&>(args)...);
      ++size_;
    }

    void pop_back() noexcept {
      assert(size_ == 0 && "can not pop back from an empty vector");
      allocator_.destroy(&buf_[size_]);
      --size_;
    }

    template<typename MappingFn, typename MappedType = std::invoke_result_t<MappingFn, const T&>>
    Vector<MappedType, Allocator> map(MappingFn fn) const {
      Vector<MappedType, Allocator> mapped;
      mapped.reallocate(size_);
      for (std::size_t i = 0; i < size_; ++i) {
        mapped.push_back(fn(buf_[i]));
      }
      return mapped;
    }

    template <typename FilterFn>
    Vector filter(FilterFn fn) const {
      static_assert(std::is_same_v<std::invoke_result_t<FilterFn, const T&>, bool>, "filter function must evaluate to bool");
      Vector filtered;
      for (std::size_t i = 0; i < size_; ++i) {
        if (fn(buf_[i])) {
          filtered.push_back(buf_[i]);
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
  for (std::size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, template<typename> typename A, template<typename> typename B>
bool operator!=(const flow::Vector<T, A>& lhs, const flow::Vector<T, B>& rhs) {
  return !(lhs == rhs);
}
