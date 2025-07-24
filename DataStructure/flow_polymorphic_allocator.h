#pragma once
#include "flow_default_memory_resource.h"
#include <cstddef>

namespace flow {

  /// @brief Polymorphic allocator wrapping a non-owning memory resource.
  /// Allocation strategy depends on the memory resource implementation.
  template <typename T = std::byte>
  class PolymorphicAllocator {
  public:
    using value_type = T;

    /// @brief Default constructor using the default memory resource.
    PolymorphicAllocator() noexcept
      : resource_(&DefaultMemoryResource::getResource()) {
    }

    /// @brief Construct allocator with a specific memory resource.
    /// @param resource Memory resource to use.
    explicit PolymorphicAllocator(MemoryResource& resource) noexcept
      : resource_(&resource) {
    }

    /// @brief Copy constructor from another polymorphic allocator.
    /// @param allocator Another allocator to copy from.
    template <typename U>
    PolymorphicAllocator(const PolymorphicAllocator<U>& allocator) noexcept
      : resource_(allocator.resource_) {
    }

    /// @brief Allocate memory for count objects.
    /// @param count Number of objects to allocate.
    /// @return Pointer to allocated memory.
    T* allocate(std::size_t count) {
      return static_cast<T*>(resource_->allocate(sizeof(T) * count, alignof(T)));
    }

    /// @brief Deallocate memory.
    /// @param address Pointer to memory to deallocate.
    /// @param count Number of objects previously allocated.
    void deallocate(T* address, std::size_t count) noexcept {
      resource_->deallocate(address, sizeof(T) * count, alignof(T));
    }

    /// @brief Construct an object in place at the given address.
    /// @tparam U Type of object to construct.
    /// @param address Pointer to memory.
    /// @param args Constructor arguments.
    template <typename U, typename... Args>
    void construct(U* address, Args&&... args) {
      ::new (address) U(std::forward<Args>(args)...);
    }

    /// @brief Destroy the object at the given address.
    /// @param address Pointer to object to destroy.
    template <typename U>
    void destroy(U* address) noexcept {
      address->~U();
    }

  private:
    MemoryResource* resource_; ///< Non-owning pointer to memory resource.

    template <typename U>
    friend class PolymorphicAllocator;

    template <typename U>
    friend bool operator==(const PolymorphicAllocator<T>& lhs, const PolymorphicAllocator<U>& rhs) noexcept {
      return lhs.resource_ == rhs.resource_;
    }

    template <typename U>
    friend bool operator!=(const PolymorphicAllocator<T>& lhs, const PolymorphicAllocator<U>& rhs) noexcept {
      return !(lhs == rhs);
    }
  };

}
