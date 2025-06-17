#pragma once
#include <cstddef>

namespace flow {
  template <typename T>
  class BasicAllocator {
  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    T* allocate(std::size_t count) const {
      return static_cast<T*>(operator new(count * sizeof(T)));
    }

    void deallocate(T* ptr, std::size_t) const noexcept {
      operator delete(ptr);
    }

    template <typename U, typename ...Args>
    void construct(U* ptr, Args&&... args) const {
      new(ptr) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* ptr) const noexcept {
      ptr->~U();
    }
  };

  template <typename T, typename U>
  bool operator==(const BasicAllocator<T>&, const BasicAllocator<U>&) {
    return true;
  }

  template <typename T, typename U>
  bool operator!=(const BasicAllocator<T>&, const BasicAllocator<U>&) {
    return false;
  }
}
