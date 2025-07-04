#pragma once
#include "default_memory_resource.h"

#include <cstddef>

namespace flow::pmr {

  // An adaptor wraps around a non-owning memory resource.
  // It provides accessor to allocate/deallocate memory,
  // or construct/destroy objects.
  template <typename T>
  class PolymorphicAllocator {
  public:
    using value_type = T;
    using pointer_type = T*;
    using reference_type = T&;
    using const_pointer_type = const T*;
    using const_reference_type = const T&;

    PolymorphicAllocator()
      : resource_(&DefaultMemoryResource::getResource()) {
    }

    explicit PolymorphicAllocator(MemoryResource& resource)
      : resource_(&resource) {
    }

    template <typename U>
    PolymorphicAllocator(const PolymorphicAllocator<U>& allocator) noexcept
      : resource_(allocator.resource_) {
    }

    T* allocate(size_t n) {
      return static_cast<T*>(resource_->allocate(sizeof(T) * n, alignof(T)));
    }

    void deallocate(T* address, size_t n) noexcept {
      resource_->deallocate(address, sizeof(T) * n, alignof(T));
    }

    template <typename U, typename... Args>
    void construct(U* address, Args&&... args) {
      ::new(address) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* address) noexcept {
      address->~U();
    }

  private:
    MemoryResource* resource_; // Non-owning memory resource.
  };
}
