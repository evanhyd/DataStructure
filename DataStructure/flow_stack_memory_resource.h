#pragma once
#include "flow_memory_algorithm.h"
#include "flow_memory_resource.h"
#include <cassert>
#include <cstddef>
#include <exception>

namespace flow {

  /// @brief
  /// A stack-based memory resource that allocates memory in a LIFO order from a fixed buffer.
  /// Deallocation must happen in reverse order of allocation.
  /// Throws std::bad_alloc if there is insufficient space for an allocation.
  class StackMemoryResource : public MemoryResource {
  public:
    explicit StackMemoryResource(void* buffer, std::size_t capacity) noexcept
      : buffer_(buffer), capacity_(capacity) {
    }

  protected:
    struct Header {
      void* oldBuffer;
    };

    void* buffer_;
    std::size_t capacity_;

    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) override {
      void* oldBuffer = buffer_;

      void* alignedHeader = flow::alignWithHeader<Header>(alignment, bytes, buffer_, capacity_);
      if (!alignedHeader) {
        throw std::bad_alloc();
      }

      new (alignedHeader) Header(oldBuffer);
      void* allocatedBlock = reinterpret_cast<Header*>(alignedHeader) + 1;
      buffer_ = reinterpret_cast<std::byte*>(allocatedBlock) + bytes;
      capacity_ -= sizeof(Header) + bytes;
      return allocatedBlock;
    }

    virtual void deallocateImp(void* address, std::size_t bytes, [[maybe_unused]] std::size_t alignment) override {
      // TODO: assert address is the top most valid address
      assert(address == nullptr || address <= buffer_);
      if (!address) {
        return;
      }

      Header* header = reinterpret_cast<Header*>(address) - 1;
      std::size_t padding = pointerDistance(header->oldBuffer, header);
      buffer_ = header->oldBuffer;
      capacity_ += padding + sizeof(Header) + bytes;
      header->~Header();
    }
  };
}
