#include "../DataStructure/memory_algorithm.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>

using namespace flow::pmr;

struct NoDefaultType {
  int a, b;
  NoDefaultType(int v) : a(v), b(v) {}
};

TEST(UninitializedCopyTest, CopiesElements) {
  std::allocator<int> alloc;

  int source[] = { 1, 2, 3, 4, 5 };
  int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 5);

  uninitializedCopy(alloc, std::begin(source), std::end(source), dest);

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(dest[i], source[i]);
  }

  destroyElements(alloc, dest, dest + 5);
  std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 5);
}

TEST(UninitializedMoveTest, MovesElements) {
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

TEST(UninitializedFillTest, FillsElementsWithValue) {
  std::allocator<int> alloc;
  int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 4);

  uninitializedFill(alloc, dest, dest + 4, 42);

  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(dest[i], 42);
  }

  destroyElements(alloc, dest, dest + 4);
  std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 4);
}
TEST(UninitializedEmplaceTest, ConstructsDefaultUniquePtr) {
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

TEST(UninitializedEmplaceTest, ConstructNoDefaultWithArgs) {
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

TEST(UninitializedCopyNTest, CopiesNElements) {
  std::allocator<int> alloc;
  int source[] = { 10, 20, 30, 40, 50 };

  int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

  uninitializedCopyN(alloc, std::begin(source), 3, dest);

  EXPECT_EQ(dest[0], 10);
  EXPECT_EQ(dest[1], 20);
  EXPECT_EQ(dest[2], 30);

  destroyElements(alloc, dest, dest + 3);
  std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
}

TEST(UninitializedMoveNTest, MovesNElements) {
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

TEST(UninitializedFillNTest, FillsNElements) {
  std::allocator<int> alloc;
  int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 5);

  uninitializedFillN(alloc, dest, 5, 99);

  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(dest[i], 99);
  }

  destroyElements(alloc, dest, dest + 5);
  std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 5);
}

TEST(DestroyElementsNTest, DestroysElements) {
  std::allocator<int> alloc;
  int* dest = std::allocator_traits<decltype(alloc)>::allocate(alloc, 3);

  uninitializedFillN(alloc, dest, 3, 123);
  destroyElementsN(alloc, dest, 3);
  std::allocator_traits<decltype(alloc)>::deallocate(alloc, dest, 3);
}

TEST(DeleteBufferTest, DeletesBuffer) {
  std::allocator<int> alloc;

  int* buffer = std::allocator_traits<decltype(alloc)>::allocate(alloc, 4);
  uninitializedFillN(alloc, buffer, 2, 555);
  deleteBuffer(alloc, buffer, 2, 4);
}

TEST(UninitializedEmplaceNTest, ConstructsDefaultUniquePtr) {
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

TEST(UninitializedEmplaceNTest, ConstructNoDefaultWithArgs) {
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
