#include "../DataStructure/flow_buddy_memory_resource.h"
#include "flow_memory_resource_testsuit.h"
#include <gtest/gtest.h>

namespace flow {
  class BuddyMemoryResourceTest : public ::testing::Test {
  protected:
    static constexpr std::size_t kBufferSize = 4096;
    static constexpr std::size_t kBufferAlignment = 64;

    std::byte buffer[kBufferSize];
    BuddyMemoryResource resource{ buffer, kBufferSize, kBufferAlignment };
  };

  // Common tests.
  TEST_F(BuddyMemoryResourceTest, TestAllocateInt) {
    testAllocateInt(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateIntDistinct) {
    testAllocateIntDistinct(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateIntArray) {
    testAllocateIntArray(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateFoo) {
    testAllocateFoo(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateFooDistinct) {
    testAllocateFooDistinct(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateFooArray) {
    testAllocateFooArray(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateAlignasBigFoo) {
    testAllocateAlignasBigFoo(resource);
  }

  TEST_F(BuddyMemoryResourceTest, TestAllocateAlignasSmallFoo) {
    testAllocateAlignasSmallFoo(resource);
  }
}
