#pragma once
#include "flow_memory_resource.h"
#include <cstddef>
#include <exception>
#include <memory>

namespace flow {
  class StackMemoryResource : public MemoryResource {
  public:
    explicit StackMemoryResource(void* buffer, std::size_t capacity) noexcept
      : buffer_(buffer), capacity_(capacity) {
    }

  private:
    struct StackNode {
      uint8_t padding;
    };

    void* buffer_;
    std::size_t capacity_;

    virtual void* allocateImp(size_t bytes, size_t alignment) override {
      ++bytes;
    }

    virtual void deallocateImp(
      [[maybe_unused]] void* address,
      [[maybe_unused]] size_t bytes,
      [[maybe_unused]] size_t alignment) override {
    }
  };
}
