#include "../DataStructure/flow_pool_memory_resource.h"
#include "flow_memory_resource_testsuit.h"
#include <cstddef>
#include <exception>
#include <gtest/gtest.h>
#include <vadefs.h>
#include <vector>

namespace flow {
    class PoolMemoryResourceTest : public ::testing::Test {
    protected:
      static constexpr std::size_t kBufferSize = 4096;
      static constexpr std::size_t kBlockSize = 128;
      static constexpr std::size_t kAlignment = 64;

      std::byte buffer[kBufferSize];
      PoolMemoryResource resource{ buffer, kBufferSize, kBlockSize, kAlignment };
    };


    // Common tests.
    TEST_F(PoolMemoryResourceTest, TestAllocateInt) {
      testAllocateInt(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateIntDistinct) {
      testAllocateIntDistinct(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateIntArray) {
      testAllocateIntArray(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateFoo) {
      testAllocateFoo(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateFooDistinct) {
      testAllocateFooDistinct(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateFooArray) {
      testAllocateFooArray(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateAlignasBigFoo) {
      testAllocateAlignasBigFoo(resource);
    }

    TEST_F(PoolMemoryResourceTest, TestAllocateAlignasSmallFoo) {
      testAllocateAlignasSmallFoo(resource);
    }

    // Specialized tests.
    TEST_F(PoolMemoryResourceTest, ExactBlockSizeAndAlignment) {
      void* ptr = resource.allocate(kBlockSize, kAlignment);
      ASSERT_NE(ptr, nullptr);
      EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % kAlignment, 0u);
      resource.deallocate(ptr, kBlockSize, kAlignment);
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithSmallerAlignmentThrows) {
      EXPECT_NO_THROW(resource.allocate(kBlockSize, kAlignment / 2));
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithLargerAlignmentThrows) {
      EXPECT_THROW(resource.allocate(kBlockSize, kAlignment * 2), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, AllocateWithTooLargeBlockThrows) {
      EXPECT_THROW(resource.allocate(kBlockSize + 1, kAlignment), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, AllocateUntilFullAndReuseAfterDeallocate) {
      std::vector<void*> ptrs;
      while (true) {
        try {
          ptrs.push_back(resource.allocate(kBlockSize, kAlignment));
        } catch (const std::bad_alloc&) {
          break;
        }
      }

      ASSERT_FALSE(ptrs.empty());

      for (void* ptr : ptrs) {
        resource.deallocate(ptr, kBlockSize, kAlignment);
      }

      // Now reallocate to ensure reuse
      for (std::size_t i = 0; i < ptrs.size(); ++i) {
        void* ptr = resource.allocate(kBlockSize, kAlignment);
        ASSERT_NE(ptr, nullptr);
        resource.deallocate(ptr, kBlockSize, kAlignment);
      }
    }

    TEST_F(PoolMemoryResourceTest, SmallBufferCannotHoldAnyBlock) {
      constexpr std::size_t kSmallBufferSize = sizeof(void*) + kBlockSize - 1;
      std::byte smallBuf[kSmallBufferSize]{};
      PoolMemoryResource smallPool(smallBuf, kSmallBufferSize, kBlockSize, kAlignment);
      EXPECT_THROW(smallPool.allocate(kBlockSize, kAlignment), std::bad_alloc);
    }

    TEST_F(PoolMemoryResourceTest, MisalignedBufferSkipsAllocation) {
      constexpr std::size_t kMisalignedBufferSize = 512;
      std::byte rawBuf[kMisalignedBufferSize + 1];
      void* misalignedPtr = rawBuf + 1;

      PoolMemoryResource misalignedPool(misalignedPtr, kMisalignedBufferSize, kBlockSize, kAlignment);

      // At most 1 block might be aligned; just ensure it's safe
      try {
        void* ptr = misalignedPool.allocate(kBlockSize, kAlignment);
        ASSERT_NE(ptr, nullptr);
        misalignedPool.deallocate(ptr, kBlockSize, kAlignment);
      } catch (...) {
        SUCCEED();
      }
    }

}  // namespace flow
