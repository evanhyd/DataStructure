#pragma once
#include "flow_memory_resource.h"
#include <cassert>
#include <cstddef>
#include <exception>
#include <memory>

namespace flow {
  class ArenaMemoryResource : public MemoryResource {
  public:
    explicit ArenaMemoryResource(void* buffer, size_t capacity) noexcept
      : beginBuffer_(buffer), buffer_(buffer), capacity_(capacity) {
    }
  
  protected:
    const void* beginBuffer_;
    void* buffer_;
    size_t capacity_;

    virtual void* allocateImp(size_t bytes, size_t alignment) override {
      void* aligned = std::align(alignment, bytes, buffer_, capacity_);
      if (!aligned || capacity_ < bytes) {
        throw std::bad_alloc();
      }

      buffer_ = static_cast<std::byte*>(aligned) + bytes;
      capacity_ -= bytes;
      return aligned;
    }

    virtual void deallocateImp(
      [[maybe_unused]] void* address,
      [[maybe_unused]] size_t bytes,
      [[maybe_unused]] size_t alignment) override {
      assert(address == nullptr || (beginBuffer_ <= address && address <= buffer_));
    }
  };
}
