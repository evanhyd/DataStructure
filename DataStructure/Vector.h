#pragma once
#include <cassert>
#include <type_traits>
#include <initializer_list>
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
      assert(capacity > 0 && "requested 0 capacity");
      
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

    void deleteAll() noexcept {
      for (std::size_t i = 0; i < size_; ++i) {
        allocator_.destroy(&buf_[i]);
      }
      allocator_.deallocate(buf_, capacity_);
    }

  public:
    using value_type = T;

    Vector()
      : allocator_(Allocator<T>()), buf_(nullptr), size_(0), capacity_(0) {
    }

    Vector(const Vector<T>& rhs) 
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(rhs.size_)), size_(rhs.size_), capacity_(rhs.size_) {
      for (std::size_t i = 0; i < size_; ++i) {
        allocator_.construct(&buf_[i], rhs.buf_[i]);
      }
    }

    Vector(Vector<T>&& rhs) noexcept
      : allocator_(std::exchange(rhs.allocator_, Allocator<T>())),
        buf_(std::exchange(rhs.buf_, nullptr)), size_(std::exchange(rhs.size_, 0)), capacity_(std::exchange(rhs.capacity_, 0)) {
    }

    Vector(std::initializer_list<T> list)
      : allocator_(Allocator<T>()), buf_(allocator_.allocate(list.size())), size_(list.size()), capacity_(list.size()) {
      std::size_t i = 0;
      for (auto it = list.begin(); it != list.end(); ++it) {
        allocator_.construct(&buf_[i++], *it);
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

    constexpr std::size_t size() const {
      return size_;
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

    void reserve(std::size_t capacity) {
      if (capacity > capacity_) {
        reallocate(capacity);
      }
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


    //friend functions
    friend bool operator==(const Vector& lhs, const Vector& rhs) {
      if (lhs.size() != rhs.size()) {
        return false;
      }
      for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.buf_[i] != rhs.buf_[i]) {
          return false;
        }
      }
      return true;
    }

    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
      return !(lhs == rhs);
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
