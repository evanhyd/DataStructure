#include "../DataStructure/flow_stack_memory_resource.h"
#include "flow_memory_resource_testsuit.h"
#include <cstddef>
#include <exception>
#include <gtest/gtest.h>

namespace flow {
  class StackMemoryResourceWhiteBox : public StackMemoryResource {
  public:
    using Header = StackMemoryResource::Header;
    StackMemoryResourceWhiteBox(void* buffer, std::size_t size)
      : StackMemoryResource(buffer, size), originalBuffer_(buffer), originalCapacity_(size) {
    }

    void* getCurrentBuffer() const { return buffer_; }
    void* getOriginalBuffer() const { return originalBuffer_; }
    std::size_t getRemainingCapacity() const { return capacity_; }
    std::size_t getOriginalCapacity() const { return originalCapacity_; }

  private:
    void* originalBuffer_;
    std::size_t originalCapacity_;
  };

  class StackMemoryResourceTest : public ::testing::Test {
  protected:
    static constexpr std::size_t kBufferSize = 512;
    std::byte rawBuffer[kBufferSize];
    StackMemoryResourceWhiteBox resource{ rawBuffer, kBufferSize };
  };

  // Common tests.
  TEST_F(StackMemoryResourceTest, TestAllocateInt) {
    testAllocateInt(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateIntDistinct) {
    testAllocateIntDistinct(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateIntArray) {
    testAllocateIntArray(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateFoo) {
    testAllocateFoo(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateFooDistinct) {
    testAllocateFooDistinct(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateFooArray) {
    testAllocateFooArray(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateAlignasBigFoo) {
    testAllocateAlignasBigFoo(resource);
  }

  TEST_F(StackMemoryResourceTest, TestAllocateAlignasSmallFoo) {
    testAllocateAlignasSmallFoo(resource);
  }

  // Specialized tests.
  TEST_F(StackMemoryResourceTest, AllocUpdatesBufferAndCapacity) {
    void* oldBuffer = resource.getCurrentBuffer();
    std::size_t oldCapacity = resource.getRemainingCapacity();

    void* ptr = resource.allocate(64, 16);
    ASSERT_NE(ptr, nullptr);

    void* newBuffer = resource.getCurrentBuffer();
    std::size_t newCapacity = resource.getRemainingCapacity();

    EXPECT_GT(reinterpret_cast<std::uintptr_t>(newBuffer), reinterpret_cast<std::uintptr_t>(oldBuffer));
    EXPECT_LT(newCapacity, oldCapacity);
  }

  TEST_F(StackMemoryResourceTest, DeallocRestoresBufferAndCapacity) {
    std::size_t bytes = 64;
    std::size_t align = 16;

    void* originalBuffer = resource.getCurrentBuffer();
    std::size_t originalCapacity = resource.getRemainingCapacity();

    void* ptr = resource.allocate(bytes, align);
    ASSERT_NE(ptr, nullptr);

    resource.deallocate(ptr, bytes, align);

    EXPECT_EQ(resource.getCurrentBuffer(), originalBuffer);
    EXPECT_EQ(resource.getRemainingCapacity(), originalCapacity);
  }

  TEST_F(StackMemoryResourceTest, OldBufferIsCorrectlyStored) {
    void* ptr = resource.allocate(64, 64);
    auto* bytePtr = static_cast<std::byte*>(ptr);
    auto* node = reinterpret_cast<const StackMemoryResourceWhiteBox::Header*>(bytePtr - sizeof(StackMemoryResourceWhiteBox::Header));
    EXPECT_EQ(node->oldBuffer, resource.getOriginalBuffer());
    resource.deallocate(ptr, 64, 64);
  }

  TEST_F(StackMemoryResourceTest, MultipleAllocationsTrackStackCorrectly) {
    void* a = resource.allocate(16, 8);
    void* b = resource.allocate(32, 8);

    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_LT(reinterpret_cast<uintptr_t>(a), reinterpret_cast<uintptr_t>(b));

    resource.deallocate(b, 32, 8);
    resource.deallocate(a, 16, 8);

    EXPECT_EQ(resource.getCurrentBuffer(), rawBuffer);
    EXPECT_EQ(resource.getRemainingCapacity(), kBufferSize);
  }
}
