#pragma once
#include "flow_memory_algorithm.h"
#include "flow_memory_resource.h"
#include <cstddef>
#include <exception>

namespace flow {

  /// @brief
  /// A pool memory resource that manages fixed-size memory blocks from a pre-allocated buffer.
  /// The allocation size must be less or equal to the block size.
  /// The allocation alignment must be less or equal to the block alignment.
  /// Throws std::bad_alloc if the constraint is not met or run out of memory.
  class PoolMemoryResource : public MemoryResource {
  public:
    PoolMemoryResource(void* buffer, std::size_t capacity, std::size_t blockSize, std::size_t blockAlignment = sizeof(std::max_align_t))
      : blockSize_(blockSize), blockAlignment_(blockAlignment), head_(nullptr) {

      Header** headPtr = &head_;
      for (;;) {
        Header* header = alignWithHeader<Header>(blockAlignment_, blockSize_, buffer, capacity);
        if (!header) {
          break;
        }

        new (header) Header(nullptr);
        *headPtr = header;
        headPtr = &(header->next);

        buffer = reinterpret_cast<std::byte*>(buffer) + sizeof(Header) + blockSize;
        capacity -= sizeof(Header) + blockSize;
      }
    }

    ~PoolMemoryResource() {
      // May be useless, since it has a trivial destructor.
      while (head_) {
        Header* next = head_->next;
        head_->~Header();
        head_ = next;
      }
    }

  protected:
    struct Header {
      Header* next;
    };

    std::size_t blockSize_;
    std::size_t blockAlignment_;
    Header* head_;
    
    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) override {
      if (blockSize_ < bytes || blockAlignment_ < alignment || !head_) {
        throw std::bad_alloc();
      }

      void* allocatedBlock = head_ + 1;
      head_ = head_->next;
      return allocatedBlock;
    }

    virtual void deallocateImp(
      void* address,
      [[maybe_unused]] std::size_t bytes,
      [[maybe_unused]] std::size_t alignment) override {
      Header* header = reinterpret_cast<Header*>(address) - 1;
      header->next = head_;
      head_ = header;
    }
  };
}
