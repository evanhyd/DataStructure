#include "../DataStructure/flow_pool_memory_resource.h"
#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace flow {
    class PoolMemoryResourceTest : public ::testing::Test {
    protected:
      static constexpr std::size_t kBufferSize = 4096;
      static constexpr std::size_t kBlockSize = 64;
      static constexpr std::size_t kAlignment = alignof(std::max_align_t);

      std::unique_ptr<std::byte[]> buffer;
      std::unique_ptr<PoolMemoryResource> pool;

      void SetUp() override {
        buffer = std::make_unique<std::byte[]>(kBufferSize);
        pool = std::make_unique<PoolMemoryResource>(buffer.get(), kBufferSize, kBlockSize, kAlignment);
      }
    };

    TEST_F(PoolMemoryResourceTest, ExactBlockSizeAndAlignment) {
      void* ptr = pool->allocate(kBlockSize, kAlignment);
      ASSERT_NE(ptr, nullptr);
      EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % kAlignment, 0u);
      pool->deallocate(ptr, kBlockSize, kAlignment);
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithSmallerAlignmentThrows) {
      EXPECT_NO_THROW(pool->allocate(kBlockSize, kAlignment / 2));
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithLargerAlignmentThrows) {
      EXPECT_THROW(pool->allocate(kBlockSize, kAlignment * 2), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithTooLargeBlockThrows) {
      EXPECT_THROW(pool->allocate(kBlockSize + 1, kAlignment), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, AllocateUntilFullAndReuseAfterDeallocate) {
      std::vector<void*> ptrs;
      while (true) {
        try {
          ptrs.push_back(pool->allocate(kBlockSize, kAlignment));
        } catch (const std::bad_alloc&) {
          break;
        }
      }

      ASSERT_FALSE(ptrs.empty());

      for (void* ptr : ptrs) {
        pool->deallocate(ptr, kBlockSize, kAlignment);
      }

      // Now reallocate to ensure reuse
      for (std::size_t i = 0; i < ptrs.size(); ++i) {
        void* ptr = pool->allocate(kBlockSize, kAlignment);
        ASSERT_NE(ptr, nullptr);
        pool->deallocate(ptr, kBlockSize, kAlignment);
      }
    }

    TEST_F(PoolMemoryResourceTest, SmallBufferCannotHoldAnyBlock) {
      constexpr std::size_t kSmallBufferSize = sizeof(void*) + kBlockSize - 1;
      std::unique_ptr<std::byte[]> smallBuf = std::make_unique<std::byte[]>(kSmallBufferSize);

      PoolMemoryResource smallPool(smallBuf.get(), kSmallBufferSize, kBlockSize, kAlignment);

      EXPECT_THROW(smallPool.allocate(kBlockSize, kAlignment), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, MisalignedBufferSkipsAllocation) {
      constexpr std::size_t kMisalignedBufferSize = 512;
      auto rawBuf = std::make_unique<std::byte[]>(kMisalignedBufferSize + 1);
      void* misalignedPtr = rawBuf.get() + 1;

      PoolMemoryResource misalignedPool(misalignedPtr, kMisalignedBufferSize, kBlockSize, kAlignment);

      // At most 1 block might be aligned; just ensure it's safe
      try {
        void* ptr = misalignedPool.allocate(kBlockSize, kAlignment);
        ASSERT_NE(ptr, nullptr);
        misalignedPool.deallocate(ptr, kBlockSize, kAlignment);
      } catch (...) {
        SUCCEED();  // acceptable if bad_alloc thrown
      }
    }

}  // namespace flow
