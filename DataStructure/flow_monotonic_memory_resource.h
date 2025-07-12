#pragma once
#include "flow_memory_resource.h"
#include <exception>
#include <memory>

namespace flow {
  class MonotonicMemoryResource : public MemoryResource {
  public:
    MonotonicMemoryResource(void* buffer, std::size_t capacity) noexcept
      : buffer_(buffer), capacity_(capacity) {
    }
  
  private:
    void* buffer_;
    std::size_t capacity_;

    virtual void* allocateImp(size_t bytes, size_t alignment) {
      void* buffer = std::align(alignment, bytes, buffer_, capacity_);
      if (!buffer) {
        throw std::bad_alloc();
      }
      buffer_ = static_cast<byte*>(buffer) + bytes;
      return buffer;
    }

    virtual void deallocateImp(
      [[maybe_unused]] void* address,
      [[maybe_unused]] size_t bytes,
      [[maybe_unused]] size_t alignment) {
    }
  };
}
