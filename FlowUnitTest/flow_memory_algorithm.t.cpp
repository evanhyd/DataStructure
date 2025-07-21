#include "../DataStructure/flow_memory_algorithm.h"
#include <gtest/gtest.h>
#include <memory>

namespace flow {
  struct NoDefaultType {
    int a, b;
    NoDefaultType(int v) : a(v), b(v) {}
  };

  TEST(MemoryAlgorithm, UninitializedForwardTestCopiesElements) {
    std::allocator<int> alloc;

    int source[] = { 1, 2, 3, 4, 5 };
    int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 5);

    uninitializedForward(alloc, std::begin(source), std::end(source), dest);

    for (std::size_t i = 0; i < 5; ++i) {
      EXPECT_EQ(dest[i], source[i]);
    }

    destroyElements(alloc, dest, dest + 5);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 5);
  }

  TEST(MemoryAlgorithm, UninitializedForwardTestMoveElements) {
    std::allocator<std::string> alloc;

    std::string source[] = { "ab", "cd", "ef"};
    std::string* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

    uninitializedForward(alloc, std::move_iterator(std::begin(source)), std::move_iterator(std::end(source)), dest);

    EXPECT_EQ("ab", dest[0]);
    EXPECT_EQ("cd", dest[1]);
    EXPECT_EQ("ef", dest[2]);
    EXPECT_TRUE(source[0].empty());
    EXPECT_TRUE(source[1].empty());
    EXPECT_TRUE(source[2].empty());

    destroyElements(alloc, dest, dest + 3);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, UninitializedMoveTestMovesElements) {
    std::allocator<std::unique_ptr<int>> alloc;

    std::unique_ptr<int> source[] = {
        std::make_unique<int>(1),
        std::make_unique<int>(2),
        std::make_unique<int>(3)
    };

    auto* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);
    uninitializedMove(alloc, std::begin(source), std::begin(source) + 2, dest);

    EXPECT_EQ(source[0], nullptr);
    EXPECT_EQ(source[1], nullptr);
    EXPECT_NE(source[2], nullptr);
    EXPECT_NE(dest[0], nullptr);
    EXPECT_NE(dest[1], nullptr);

    destroyElements(alloc, dest, dest + 2);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, UninitializedFillTestFillsElementsWithValue) {
    std::allocator<int> alloc;
    int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 4);

    uninitializedFill(alloc, dest, dest + 4, 42);

    for (int i = 0; i < 4; ++i) {
      EXPECT_EQ(dest[i], 42);
    }

    destroyElements(alloc, dest, dest + 4);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 4);
  }
  TEST(MemoryAlgorithm, UninitializedEmplaceTestConstructsDefaultUniquePtr) {
    using AllocatorType = std::allocator<std::unique_ptr<int>>;

    AllocatorType alloc;
    auto* buffer = std::allocator_traits<AllocatorType>::allocate(alloc, 3);
    auto* dest = buffer;
    uninitializedEmplace(alloc, dest, dest + 3);

    for (std::size_t i = 0; i < 3; ++i) {
      EXPECT_EQ(dest[i], nullptr);
    }

    for (std::size_t i = 0; i < 3; ++i) {
      std::allocator_traits<AllocatorType>::destroy(alloc, &dest[i]);
    }

    std::allocator_traits<AllocatorType>::deallocate(alloc, buffer, 3);
  }

  TEST(MemoryAlgorithm, UninitializedEmplaceTestConstructNoDefaultWithArgs) {
    using AllocatorType = std::allocator<NoDefaultType>;

    AllocatorType alloc;
    auto* buffer = std::allocator_traits<AllocatorType>::allocate(alloc, 3);
    auto* dest = buffer;
    uninitializedEmplace(alloc, dest, dest + 3, 20);

    for (std::size_t i = 0; i < 3; ++i) {
      EXPECT_EQ(dest[i].a, 20);
      EXPECT_EQ(dest[i].b, 20);
    }

    for (std::size_t i = 0; i < 3; ++i) {
      std::allocator_traits<AllocatorType>::destroy(alloc, &dest[i]);
    }

    std::allocator_traits<AllocatorType>::deallocate(alloc, buffer, 3);
  }

  TEST(MemoryAlgorithm, UninitializedForwardNTestCopiesNElements) {
    std::allocator<int> alloc;
    int source[] = { 10, 20, 30, 40, 50 };

    int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

    uninitializedForwardN(alloc, std::begin(source), 3, dest);

    EXPECT_EQ(dest[0], 10);
    EXPECT_EQ(dest[1], 20);
    EXPECT_EQ(dest[2], 30);

    destroyElements(alloc, dest, dest + 3);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, UninitializedForwardNTestMovesNElements) {
    std::allocator<std::string> alloc;

    std::string source[] = { "ab", "cd", "ef" };
    std::string* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

    uninitializedForwardN(alloc, std::move_iterator(std::begin(source)), 3, dest);

    EXPECT_EQ("ab", dest[0]);
    EXPECT_EQ("cd", dest[1]);
    EXPECT_EQ("ef", dest[2]);
    EXPECT_TRUE(source[0].empty());
    EXPECT_TRUE(source[1].empty());
    EXPECT_TRUE(source[2].empty());

    destroyElements(alloc, dest, dest + 3);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, UninitializedMoveNTestMovesNElements) {
    std::allocator<std::unique_ptr<int>> alloc;

    std::unique_ptr<int> source[] = {
        std::make_unique<int>(100),
        std::make_unique<int>(200),
        std::make_unique<int>(300)
    };

    auto* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

    uninitializedMoveN(alloc, std::begin(source), 2, dest);

    EXPECT_EQ(source[0], nullptr);
    EXPECT_EQ(source[1], nullptr);
    EXPECT_NE(source[2], nullptr);
    EXPECT_NE(dest[0], nullptr);
    EXPECT_NE(dest[1], nullptr);

    destroyElements(alloc, dest, dest + 2);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, UninitializedFillNTestFillsNElements) {
    std::allocator<int> alloc;
    int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 5);

    uninitializedFillN(alloc, dest, 5, 99);

    for (int i = 0; i < 5; ++i) {
      EXPECT_EQ(dest[i], 99);
    }

    destroyElements(alloc, dest, dest + 5);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 5);
  }

  TEST(MemoryAlgorithm, DestroyElementsNTestDestroysElements) {
    std::allocator<int> alloc;
    int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

    uninitializedFillN(alloc, dest, 3, 123);
    destroyElementsN(alloc, dest, 3);
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
  }

  TEST(MemoryAlgorithm, DeleteBufferTestDeletesBuffer) {
    std::allocator<int> alloc;

    int* buffer = std::allocator_traits<decltype(alloc)>::allocate(alloc, 4);
    uninitializedFillN(alloc, buffer, 2, 555);
    deleteBuffer(alloc, buffer, 2, 4);
  }

  TEST(MemoryAlgorithm, UninitializedEmplaceNTestConstructsDefaultUniquePtr) {
    using AllocatorType = std::allocator<std::unique_ptr<int>>;

    AllocatorType alloc;
    auto* buffer = std::allocator_traits<AllocatorType>::allocate(alloc, 3);
    auto* dest = buffer;
    uninitializedEmplaceN(alloc, dest, 3);

    for (std::size_t i = 0; i < 3; ++i) {
      EXPECT_EQ(dest[i], nullptr);
    }

    for (std::size_t i = 0; i < 3; ++i) {
      std::allocator_traits<AllocatorType>::destroy(alloc, &dest[i]);
    }

    std::allocator_traits<AllocatorType>::deallocate(alloc, buffer, 3);
  }

  TEST(MemoryAlgorithm, UninitializedEmplaceNTestConstructNoDefaultWithArgs) {
    using AllocatorType = std::allocator<NoDefaultType>;

    AllocatorType alloc;
    auto* buffer = std::allocator_traits<AllocatorType>::allocate(alloc, 3);
    auto* dest = buffer;
    uninitializedEmplaceN(alloc, dest, 3, 10);

    for (std::size_t i = 0; i < 3; ++i) {
      EXPECT_EQ(dest[i].a, 10);
      EXPECT_EQ(dest[i].b, 10);
    }

    for (std::size_t i = 0; i < 3; ++i) {
      std::allocator_traits<AllocatorType>::destroy(alloc, &dest[i]);
    }

    std::allocator_traits<AllocatorType>::deallocate(alloc, buffer, 3);
  }

  TEST(MemoryAlgorithm, PointerDistanceZero) {
    std::byte buffer[256]{};
    std::byte* ptr = buffer;
    EXPECT_EQ(pointerDistance(ptr, ptr), 0);
  }

  TEST(MemoryAlgorithm, PointerDistancePowerOfTwo) {
    std::byte buffer[256]{};
    std::byte* first = buffer;
    std::byte* last = buffer + 16;
    EXPECT_EQ(pointerDistance(first, last), 16);
  }

  TEST(MemoryAlgorithm, PointerDistanceNonPowerOfTwo) {
    std::byte buffer[256]{};
    std::byte* first = buffer;
    std::byte* last = buffer + 13;
    EXPECT_EQ(pointerDistance(first, last), 13);
  }

  struct Header {
    std::int32_t a;
    std::int32_t b;
  };

  TEST(MemoryAlgorithm, AlignWithHeader_HeaderAndBufferAligned) {
    alignas(std::max_align_t) std::byte raw[256]{};
    void* ptr = raw;
    std::size_t capacity = sizeof(raw);
    constexpr std::size_t kAlignment = 64;
    constexpr std::size_t kSize = 64;  // size >= alignment

    void* result = alignWithHeader<Header>(kAlignment, kSize, ptr, capacity);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(result) % alignof(Header), 0);

    void* buffer_after_header = static_cast<std::byte*>(result) + sizeof(Header);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(buffer_after_header) % kAlignment, 0);
  }

  TEST(MemoryAlgorithm, AlignWithHeader_FailsOnInsufficientCapacity) {
    std::byte raw[32]{};
    void* ptr = raw;
    std::size_t capacity = sizeof(raw);

    constexpr std::size_t kAlignment = 64;
    constexpr std::size_t kSize = 64;  // size >= alignment, > capacity to trigger failure

    void* result = alignWithHeader<Header>(kAlignment, kSize, ptr, capacity);
    EXPECT_EQ(result, nullptr);
  }

  TEST(MemoryAlgorithm, AlignWithHeader_CustomAlignmentGreaterThanHeader) {
    alignas(std::max_align_t) std::byte raw[256]{};
    void* ptr = raw;
    std::size_t capacity = sizeof(raw);

    constexpr std::size_t kAlignment = 128;
    constexpr std::size_t kSize = 128;  // size >= alignment

    void* result = alignWithHeader<Header>(kAlignment, kSize, ptr, capacity);
    ASSERT_NE(result, nullptr);

    void* buffer_after_header = static_cast<std::byte*>(result) + sizeof(Header);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(buffer_after_header) % kAlignment, 0);
  }

  TEST(MemoryAlgorithm, AlignWithHeader_UsesExactFit) {
    constexpr std::size_t kAlignment = 32;
    constexpr std::size_t kSize = 32;  // size == alignment
    constexpr std::size_t kTotalCapacity = 256;

    alignas(std::max_align_t) std::byte raw[kTotalCapacity]{};
    void* ptr = raw;
    std::size_t capacity = kTotalCapacity;

    void* result = alignWithHeader<Header>(kAlignment, kSize, ptr, capacity);
    ASSERT_NE(result, nullptr);

    void* buffer_after_header = static_cast<std::byte*>(result) + sizeof(Header);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(buffer_after_header) % kAlignment, 0);
  }

  TEST(MemoryAlgorithm, AlignWithHeader_HeaderAlignmentFallback) {
    alignas(std::max_align_t) std::byte raw[256]{};
    void* ptr = raw;
    std::size_t capacity = sizeof(raw);

    constexpr std::size_t kAlignment = 4;
    constexpr std::size_t kSize = 4;  // size == alignment

    void* result = alignWithHeader<Header>(kAlignment, kSize, ptr, capacity);
    ASSERT_NE(result, nullptr);

    void* buffer_after_header = static_cast<std::byte*>(result) + sizeof(Header);

    // alignment falls back to alignof(Header)
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(result) % alignof(Header), 0);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(buffer_after_header) % alignof(Header), 0);
  }
}
