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
    PoolMemoryResource(void* buffer, std::size_t bufferSize, std::size_t blockSize, std::size_t blockAlignment = sizeof(std::max_align_t))
      : blockSize_(std::max(sizeof(Header), blockSize)),
        blockAlignment_(std::max(alignof(Header), blockAlignment)),
        head_(nullptr) {

      // Pad the size so all the successive blocks are all aligned if the first block is aligned.
      if (size_t tail = blockSize_ % blockAlignment_; tail > 0) {
        blockSize_ += blockAlignment_ - tail;
      }

      // Align the first block.
      head_ = static_cast<Header*>(std::align(blockAlignment_, blockSize_, buffer, bufferSize));
      if (!head_) {
        return;
      }
      Header** nextPtr = &head_->next;
      bufferSize -= blockSize_;

      for (;bufferSize >= blockSize_; bufferSize -= blockSize_) {
        buffer = static_cast<char*>(buffer) + blockSize_;
        *nextPtr = static_cast<Header*>(buffer);
        nextPtr = &static_cast<Header*>(buffer)->next;
      }
      *nextPtr = nullptr;
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

      void* block = head_;
      head_ = head_->next;
      return block;
    }

    virtual void deallocateImp(void* address, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) override {
      if (!address) {
        return;
      }
      Header* header = static_cast<Header*>(address);
      header->next = head_;
      head_ = header;
    }
  };
}
