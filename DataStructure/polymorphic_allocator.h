#pragma once
#include "default_memory_resource.h"

#include <cstddef>

namespace flow {

  // An adaptor wraps around a non-owning memory resource.
  // It provides accessor to allocate/deallocate memory,
  // or construct/destroy objects.
  template <typename T = std::byte>
  class PolymorphicAllocator {
  public:
    using value_type = T; // Minimum requirement for allocator_traits

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

    template <typename U>
    friend class PolymorphicAllocator;

    template <typename U>
    friend bool operator==(const flow::PolymorphicAllocator<T>& lhs, const flow::PolymorphicAllocator<U>& rhs) {
      return lhs.resource_ == rhs.resource_;
    }

    template <typename U>
    friend bool operator!=(const flow::PolymorphicAllocator<T>& lhs, const flow::PolymorphicAllocator<U>& rhs) {
      return !(lhs == rhs);
    }
  };
}
