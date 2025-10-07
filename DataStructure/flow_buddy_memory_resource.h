#pragma once
#include "flow_memory_resource.h"
#include <bit>
#include <cassert>
#include <cstddef>
#include <exception>
#include <algorithm>

namespace flow {

  /// @brief Fixed-size buddy allocator implementing the MemoryResource interface.
  /// The min block size is 1 byte, the maximum is 1<<64 bytes.
  class BuddyMemoryResource : public MemoryResource {
    struct BuddyBlock {
      BuddyBlock* next;
    };

    static constexpr std::size_t kMaxLevel = 64;

    std::byte* beginBuffer_;
    std::size_t capacity_;
    BuddyBlock freeList_[kMaxLevel];

    std::size_t getLevelSize(std::size_t level) const noexcept {
      return 1ull << level;
    }

    BuddyBlock* popFront(std::size_t level) noexcept {
      assert(freeList_[level].next != nullptr && "pop front from an empty list");
      BuddyBlock* block = freeList_[level].next;
      freeList_[level].next = block->next;
      return block;
    }

    void pushFront(BuddyBlock* block, std::size_t level) noexcept {
      assert(level < kMaxLevel && "level is larger than the maximum possible level");
      block->next = freeList_[level].next;
      freeList_[level].next = block;
    }

    bool isEmpty(std::size_t level) const noexcept {
      return !freeList_[level].next;
    }

    BuddyBlock* getBuddy(BuddyBlock* block, std::size_t blockSize) const noexcept {
      std::size_t index = reinterpret_cast<std::byte*>(block) - beginBuffer_;
      std::size_t buddyIndex = index ^ blockSize;
      if (buddyIndex >= capacity_) {
        return nullptr;
      }

      BuddyBlock* buddyBlock = reinterpret_cast<BuddyBlock*>(beginBuffer_ + buddyIndex);
      return buddyBlock;
    }

    bool eraseBlock(BuddyBlock* block, std::size_t level) noexcept {
      BuddyBlock* curr = &freeList_[level];
      while (curr->next) {
        BuddyBlock* child = curr->next;
        if (child == block) {
          curr->next = child->next;
          return true;
        }
        curr = child;
      }
      return false;
    }

  public:

    /// @brief Constructs a buddy allocator over a user-provided buffer.
    /// @param buffer Pointer to buffer.
    /// @param capacity Buffer size in bytes.
    /// @param alignment Required alignment.
    /// @throws std::bad_alloc If buffer cannot satisfy alignment.
    explicit BuddyMemoryResource(void* buffer, std::size_t capacity, std::size_t alignment = alignof(std::max_align_t))
      : freeList_() {

      // Not enough capacity after alignment.
      if (!std::align(alignment, alignment, buffer, capacity)) {
        throw std::bad_alloc();
      }
      beginBuffer_ = static_cast<std::byte*>(buffer);
      capacity_ = std::bit_floor(capacity);

      assert(capacity_ > 0 && "buffer capacity must be non-zero after alignment");
      std::size_t level = std::countr_zero(capacity_);
      pushFront(reinterpret_cast<BuddyBlock*>(beginBuffer_), level);
    }

    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) override {
      std::size_t requiredLevel = std::countr_zero(std::bit_ceil(std::max(bytes, alignment)));

      // Find the smallest available block.
      std::size_t level = requiredLevel;
      while (level < kMaxLevel && isEmpty(level)) {
        ++level;
      }

      // All blocks are not big enough.
      if (level >= kMaxLevel) {
        throw std::bad_alloc();
      }

      BuddyBlock* block = popFront(level);
      while (level > requiredLevel) {
        --level;
        const std::size_t levelSize = getLevelSize(level);

        BuddyBlock* secondHalf = reinterpret_cast<BuddyBlock*>(reinterpret_cast<std::byte*>(block) + levelSize);
        pushFront(secondHalf, level);
      }

      return block;
    }

    virtual void deallocateImp(void* address, std::size_t bytes, std::size_t alignment) override {
      if (!address) {
        return;
      }

      std::size_t level = std::countr_zero(std::bit_ceil(std::max(bytes, alignment)));
      for (; level < kMaxLevel; ++level) {
        BuddyBlock* buddy = getBuddy(static_cast<BuddyBlock*>(address), getLevelSize(level));
        if (buddy && eraseBlock(buddy, level)) {
          // Coalesce two blocks.
          address = std::min(address, static_cast<void*>(buddy));
        } else {
          // Push it to the free list.
          pushFront(static_cast<BuddyBlock*>(address), level);
          break;
        }
      }
    }
  };
}
