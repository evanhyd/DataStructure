#pragma once
#include <cstddef>

namespace flow {

  /// <summary>
  /// A memory resource holder interface for the PolymorphicAllocator.
  /// Responsible for allocate and deallocate raw memory.
  /// </summary>
  class MemoryResource {
  public:
    virtual ~MemoryResource() = default;

    void* allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t)) {
      return allocateImp(bytes, alignment);
    }

    void deallocate(void* address, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t)) {
      deallocateImp(address, bytes, alignment);
    }

  protected:
    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) = 0;
    virtual void deallocateImp(void* address, std::size_t bytes, std::size_t alignment) = 0;
  };
}
