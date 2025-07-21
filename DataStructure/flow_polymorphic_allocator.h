#pragma once
#include "flow_default_memory_resource.h"
#include <cstddef>

namespace flow {

  /// <summary>
  /// A polymorphic allocator that wraps around a non-owning memory resource.
  /// Memory allocation strategy is decided by memory resource's implementation.
  /// </summary>
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

    /// <summary>
    /// Allocate raw memory that can contain at least count number of objects.
    /// </summary>
    /// <param name="count">Number of objects to allocate</param>
    /// <returns>The raw memory.</returns>
    T* allocate(std::size_t count) {
      return static_cast<T*>(resource_->allocate(sizeof(T) * count, alignof(T)));
    }

    /// <summary>
    /// Deallocates allocated raw memory.
    /// </summary>
    /// <param name="address">Raw memory to deallocate.</param>
    /// <param name="count">Number of objects allocated.</param>
    void deallocate(T* address, std::size_t count) noexcept {
      resource_->deallocate(address, sizeof(T) * count, alignof(T));
    }

    /// <summary>
    /// Constructs an object in place at the given address.
    /// </summary>
    template <typename U, typename... Args>
    void construct(U* address, Args&&... args) {
      ::new(address) U(std::forward<Args>(args)...);
    }

    /// <summary>
    /// Destroys the object at the given address.
    /// </summary>
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
