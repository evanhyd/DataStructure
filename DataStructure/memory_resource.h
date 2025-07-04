#pragma once
#include <cstddef>

namespace flow::pmr {

  // A memory resource holder interface.
  // Responsible for allocate and deallocate raw memory.
  class MemoryResource {
  public:
    virtual ~MemoryResource() = default;

    void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)) {
      return allocateImp(bytes, alignment);
    }

    void deallocate(void* address, size_t bytes, size_t alignment = alignof(std::max_align_t)) {
      deallocateImp(address, bytes, alignment);
    }

  protected:
    virtual void* allocateImp(size_t bytes, size_t alignment) = 0;
    virtual void deallocateImp(void* address, size_t bytes, size_t alignment) = 0;
  };
}
