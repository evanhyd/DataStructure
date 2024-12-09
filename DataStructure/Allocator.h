#pragma once
#include <type_traits>

namespace flow {
  template <typename T>
  class BasicAllocator {

  public:
    using value_type = T;

    constexpr BasicAllocator() = default;

    template <typename U>
    constexpr BasicAllocator(const BasicAllocator<U>&) noexcept { }

    T* allocate(std::size_t n) const {
      return static_cast<T*>(operator new(n * sizeof(T)));
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

    friend bool operator==(const BasicAllocator&, const BasicAllocator&) {
      return true;
    }

    friend bool operator!=(const BasicAllocator&, const BasicAllocator&) {
      return false;
    }
  };
}
