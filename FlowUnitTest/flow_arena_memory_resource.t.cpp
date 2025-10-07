#include "../DataStructure/flow_arena_memory_resource.h"
#include "flow_memory_resource_testsuit.h"
#include <gtest/gtest.h>
#include <memory>

namespace flow {
  class ArenaMemoryResourceWhiteBox : public flow::ArenaMemoryResource {
  public:
    using flow::ArenaMemoryResource::ArenaMemoryResource;

    void* getBuffer() const { return buffer_; }
    std::size_t getCapacity() const { return capacity_; }
  };

  class ArenaMemoryResourceTest : public ::testing::Test {
  protected:
    static constexpr std::size_t kBufferSize = 512;
    std::byte buffer[kBufferSize];
    ArenaMemoryResourceWhiteBox resource{ buffer, kBufferSize };
  };

  // Common tests.
  TEST_F(ArenaMemoryResourceTest, TestAllocateInt) {
    testAllocateInt(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateIntDistinct) {
    testAllocateIntDistinct(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateIntArray) {
    testAllocateIntArray(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateFoo) {
    testAllocateFoo(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateFooDistinct) {
    testAllocateFooDistinct(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateFooArray) {
    testAllocateFooArray(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateAlignasBigFoo) {
    testAllocateAlignasBigFoo(resource);
  }

  TEST_F(ArenaMemoryResourceTest, TestAllocateAlignasSmallFoo) {
    testAllocateAlignasSmallFoo(resource);
  }

  // Specialized tests.
  TEST_F(ArenaMemoryResourceTest, InitialState) {
    EXPECT_EQ(resource.getBuffer(), buffer);
    EXPECT_EQ(resource.getCapacity(), kBufferSize);
  }

  TEST_F(ArenaMemoryResourceTest, AllocationReducesCapacityAndAdvancesBuffer) {
    void* ptr1 = resource.allocate(64, 8);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_LT(resource.getCapacity(), kBufferSize);
    EXPECT_GT(resource.getBuffer(), buffer);

    void* ptr2 = resource.allocate(32, 8);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_LT(resource.getCapacity(), kBufferSize - 64);
    EXPECT_GT(resource.getBuffer(), ptr1);
  }

  TEST_F(ArenaMemoryResourceTest, AllocationFailsWhenCapacityInsufficient) {
    resource.allocate(kBufferSize - 8, 1);
    EXPECT_THROW(resource.allocate(16, 8), std::bad_alloc);
  }

  TEST_F(ArenaMemoryResourceTest, DeallocateAllowsNullptrAndValidAddress) {
    void* ptr = resource.allocate(64, 8);
    EXPECT_NO_THROW(resource.deallocate(ptr, 64, 8));
    EXPECT_NO_THROW(resource.deallocate(nullptr, 0, 1));
  }
}
