#pragma once
#include "flow_memory_resource.h"
#include <cassert>
#include <cstddef>
#include <exception>
#include <memory>

namespace flow {

  /// @brief
  /// A linear arena memory resource that allocates memory sequentially from a fixed buffer.
  /// Throws std::bad_alloc if there is insufficient space for the requested allocation.
  class ArenaMemoryResource : public MemoryResource {
  public:
    explicit ArenaMemoryResource(void* buffer, std::size_t capacity) noexcept
      : buffer_(buffer), capacity_(capacity) {
#ifdef _DEBUG
      beginBuffer_ = buffer;
#endif
    }
  
  protected:
#ifdef _DEBUG
    void* beginBuffer_;
#endif
    void* buffer_;
    std::size_t capacity_;

    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) override {
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
      [[maybe_unused]] std::size_t bytes,
      [[maybe_unused]] std::size_t alignment) override {
      assert(address == nullptr || (beginBuffer_ <= address && address <= buffer_));
    }
  };
}
