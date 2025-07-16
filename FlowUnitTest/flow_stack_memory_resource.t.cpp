#include "../DataStructure/flow_stack_memory_resource.h"
#include <cstddef>
#include <exception>
#include <gtest/gtest.h>

namespace flow {
  class StackMemoryResourceWhiteBox : public StackMemoryResource {
  public:
    using StackNode = StackMemoryResource::StackNode;

    StackMemoryResourceWhiteBox(void* buffer, std::size_t size)
      : StackMemoryResource(buffer, size), originalBuffer_(buffer), originalCapacity_(size) {
    }

    void* getCurrentBuffer() const { return buffer_; }
    std::size_t getRemainingCapacity() const { return capacity_; }
    void* getOriginalBuffer() const { return originalBuffer_; }
    std::size_t getOriginalCapacity() const { return originalCapacity_; }

  private:
    void* originalBuffer_;
    std::size_t originalCapacity_;
  };

  class StackMemoryResourceTest : public ::testing::Test {
  protected:
    static constexpr size_t kBufferSize = 512;
    std::byte rawBuffer[kBufferSize];
    StackMemoryResourceWhiteBox resource{ rawBuffer, kBufferSize };
  };

  TEST_F(StackMemoryResourceTest, AllocUpdatesBufferAndCapacity) {
    void* oldBuffer = resource.getCurrentBuffer();
    size_t oldCapacity = resource.getRemainingCapacity();

    void* ptr = resource.allocate(64, 16);
    ASSERT_NE(ptr, nullptr);

    void* newBuffer = resource.getCurrentBuffer();
    size_t newCapacity = resource.getRemainingCapacity();

    EXPECT_GT(reinterpret_cast<std::uintptr_t>(newBuffer), reinterpret_cast<std::uintptr_t>(oldBuffer));
    EXPECT_LT(newCapacity, oldCapacity);
  }

  TEST_F(StackMemoryResourceTest, DeallocRestoresBufferAndCapacity) {
    size_t bytes = 64;
    size_t align = 16;

    void* originalBuffer = resource.getCurrentBuffer();
    size_t originalCapacity = resource.getRemainingCapacity();

    void* ptr = resource.allocate(bytes, align);
    ASSERT_NE(ptr, nullptr);

    resource.deallocate(ptr, bytes, align);

    EXPECT_EQ(resource.getCurrentBuffer(), originalBuffer);
    EXPECT_EQ(resource.getRemainingCapacity(), originalCapacity);
  }

  TEST_F(StackMemoryResourceTest, PaddingIsCorrectlyStored) {
    void* ptr = resource.allocate(8, 64);
    auto* bytePtr = static_cast<std::byte*>(ptr);
    auto* node = reinterpret_cast<const StackMemoryResourceWhiteBox::StackNode*>(bytePtr - sizeof(StackMemoryResourceWhiteBox::StackNode));
    uint8_t padding = node->padding;

    EXPECT_LE(padding, 64);
    EXPECT_GT(padding, 0);

    resource.deallocate(ptr, 8, 64);
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
