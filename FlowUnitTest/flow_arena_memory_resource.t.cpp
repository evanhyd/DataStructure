#include "../DataStructure/flow_arena_memory_resource.h"
#include <gtest/gtest.h>
#include <cstdint>
#include <memory>

namespace flow {
  class ArenaMemoryResourceTestHelper : public flow::ArenaMemoryResource {
  public:
    using flow::ArenaMemoryResource::ArenaMemoryResource;

    void* getBuffer() const { return buffer_; }
    size_t getCapacity() const { return capacity_; }
    const void* getBeginBuffer() const { return beginBuffer_; }
  };

  class ArenaMemoryResourceWhiteBoxTest : public ::testing::Test {
  protected:
    static constexpr size_t kBufferSize = 512;
    std::byte buffer[kBufferSize];
    ArenaMemoryResourceTestHelper arena{ buffer, kBufferSize };
  };

  TEST_F(ArenaMemoryResourceWhiteBoxTest, InitialState) {
    EXPECT_EQ(arena.getBeginBuffer(), buffer);
    EXPECT_EQ(arena.getBuffer(), buffer);
    EXPECT_EQ(arena.getCapacity(), kBufferSize);
  }

  TEST_F(ArenaMemoryResourceWhiteBoxTest, AllocationReducesCapacityAndAdvancesBuffer) {
    void* ptr1 = arena.allocate(64, 8);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_LT(arena.getCapacity(), kBufferSize);
    EXPECT_GT(arena.getBuffer(), buffer);

    void* ptr2 = arena.allocate(32, 8);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_LT(arena.getCapacity(), kBufferSize - 64);
    EXPECT_GT(arena.getBuffer(), ptr1);
  }

  TEST_F(ArenaMemoryResourceWhiteBoxTest, AllocationFailsWhenCapacityInsufficient) {
    arena.allocate(kBufferSize - 8, 1);
    EXPECT_THROW(arena.allocate(16, 8), std::bad_alloc);
  }

  TEST_F(ArenaMemoryResourceWhiteBoxTest, DeallocateAllowsNullptrAndValidAddress) {
    void* ptr = arena.allocate(64, 8);
    EXPECT_NO_THROW(arena.deallocate(ptr, 64, 8));
    EXPECT_NO_THROW(arena.deallocate(nullptr, 0, 1));
  }

  TEST_F(ArenaMemoryResourceWhiteBoxTest, DeallocateInvalidAddressNoCrashRelease) {
    std::byte dummy[32];
    arena.deallocate(dummy, 32, 8);
  }
}
