#pragma once
#include "flow_memory_resource.h"
#include <cassert>
#include <cstddef>
#include <exception>
#include <memory>

namespace flow {
  class StackMemoryResource : public MemoryResource {
  public:
    explicit StackMemoryResource(void* buffer, size_t capacity) noexcept
      : buffer_(buffer), capacity_(capacity) {
    }

  protected:
    struct StackNode {
      uint8_t padding; // exclude the meta data itself
    };

    void* buffer_;
    size_t capacity_;

    virtual void* allocateImp(size_t bytes, size_t alignment) override {
      assert(alignment <= (1 << (sizeof(StackNode::padding) * 8)));

      void* oldBuffer = buffer_;
      size_t oldCapacity = capacity_;
      try {
        void* aligned = std::align(alignment, bytes, buffer_, capacity_);
        if (!aligned || capacity_ < bytes) {
          throw std::bad_alloc();
        }

        // Advance to the next aligned address so we can store the meta data.
        if (aligned == oldBuffer) {

          // Ensure enough storage for the meta data + bytes.
          if (capacity_ < bytes + alignment) {
            throw std::bad_alloc();
          }
          aligned = static_cast<std::byte*>(aligned) + alignment;
          capacity_ -= alignment;
        }

        // Calculate the padding size exclude the meta data.
        uint8_t padding = static_cast<uint8_t>(static_cast<std::byte*>(aligned) - static_cast<std::byte*>(oldBuffer) - sizeof(StackNode));
        new (static_cast<StackNode*>(aligned) - 1) StackNode(padding);
        buffer_ = static_cast<std::byte*>(aligned) + bytes;
        capacity_ -= bytes;
        return aligned;

      } catch (...) {
        buffer_ = oldBuffer;
        capacity_ = oldCapacity;
        throw;
      }
    }

    virtual void deallocateImp(void* address, size_t bytes, [[maybe_unused]] size_t alignment) override {
      // TODO: assert address is the top most valid address
      assert(address == nullptr || address <= buffer_);
      if (!address) {
        return;
      }
      StackNode* meta = reinterpret_cast<StackNode*>(address) - 1;
      uint8_t padding = meta->padding;
      meta->~StackNode();

      buffer_ = reinterpret_cast<std::byte*>(meta) - padding;
      capacity_ += bytes + sizeof(StackNode) + padding;
    }
  };
}
