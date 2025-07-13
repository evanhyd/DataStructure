#pragma once
#include "flow_memory_resource.h"
#include <cstddef>
#include <exception>
#include <memory>

namespace flow {
  class ArenaMemoryResource : public MemoryResource {
  public:
    explicit ArenaMemoryResource(void* buffer, std::size_t capacity) noexcept
      : beginBuffer_(buffer), buffer_(buffer), capacity_(capacity) {
    }
  
  private:
    const void* beginBuffer_;
    void* buffer_;
    std::size_t capacity_;

    virtual void* allocateImp(size_t bytes, size_t alignment) override {
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
      [[maybe_unused]] size_t alignment) override {
      assert(beginBuffer_ <= address && address <= buffer_);
    }
  };
}
