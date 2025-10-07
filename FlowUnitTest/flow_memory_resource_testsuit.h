#pragma warning(push)
#pragma warning(disable : 4324) // Disable alignas warning.
#include "../DataStructure/flow_memory_resource.h"
#include <gtest/gtest.h>

namespace flow {

  struct Foo {
    char b;
    int64_t c;
  };

  struct alignas(64) AlignasBigFoo {
    char b;
    int64_t c;
  };

  struct alignas(4) AlignasSmallFoo {
    char a;
    int b;
  };

  // ---------- Helper functions ----------
  inline bool isAlign(void* address, std::size_t alignment) {
    return reinterpret_cast<std::uintptr_t>(address) % alignment == 0;
  }

  inline void testAllocateInt(MemoryResource& resource) {
    int* p = static_cast<int*>(resource.allocate(sizeof(int), alignof(int)));
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(isAlign(p, alignof(int)));
    resource.deallocate(p, sizeof(int), alignof(int));
  }

  inline void testAllocateIntDistinct(MemoryResource& resource) {
    int* p1 = static_cast<int*>(resource.allocate(sizeof(int), alignof(int)));
    int* p2 = static_cast<int*>(resource.allocate(sizeof(int), alignof(int)));
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(isAlign(p1, alignof(int)));
    EXPECT_TRUE(isAlign(p2, alignof(int)));
    resource.deallocate(p2, sizeof(int), alignof(int));
    resource.deallocate(p1, sizeof(int), alignof(int));
  }

  inline void testAllocateIntArray(MemoryResource& resource) {
    constexpr std::size_t kCount = 5;
    int* arr = static_cast<int*>(resource.allocate(sizeof(int) * kCount, alignof(int)));
    ASSERT_NE(arr, nullptr);
    for (std::size_t i = 0; i < kCount; ++i) arr[i] = static_cast<int>(i * 2);
    for (std::size_t i = 0; i < kCount; ++i) EXPECT_EQ(arr[i], static_cast<int>(i * 2));
    EXPECT_TRUE(isAlign(arr, alignof(int)));
    resource.deallocate(arr, sizeof(int) * kCount, alignof(int));
  }

  inline void testAllocateFoo(MemoryResource& resource) {
    Foo* p = static_cast<Foo*>(resource.allocate(sizeof(Foo), alignof(Foo)));
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(isAlign(p, alignof(Foo)));
    resource.deallocate(p, sizeof(Foo), alignof(Foo));
  }

  inline void testAllocateFooDistinct(MemoryResource& resource) {
    Foo* p1 = static_cast<Foo*>(resource.allocate(sizeof(Foo), alignof(Foo)));
    Foo* p2 = static_cast<Foo*>(resource.allocate(sizeof(Foo), alignof(Foo)));
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(isAlign(p1, alignof(Foo)));
    resource.deallocate(p2, sizeof(Foo), alignof(Foo));
    resource.deallocate(p1, sizeof(Foo), alignof(Foo));
  }

  inline void testAllocateFooArray(MemoryResource& resource) {
    constexpr std::size_t kCount = 5;
    Foo* arr = static_cast<Foo*>(resource.allocate(sizeof(Foo) * kCount, alignof(Foo)));
    ASSERT_NE(arr, nullptr);
    for (std::size_t i = 0; i < kCount; ++i) {
      arr[i].b = static_cast<char>(i + 1);
      arr[i].c = static_cast<int64_t>(i * 100);
    }
    for (std::size_t i = 0; i < kCount; ++i) {
      EXPECT_EQ(arr[i].b, static_cast<char>(i + 1));
      EXPECT_EQ(arr[i].c, static_cast<int64_t>(i * 100));
    }
    EXPECT_TRUE(isAlign(arr, alignof(Foo)));
    resource.deallocate(arr, sizeof(Foo) * kCount, alignof(Foo));
  }

  inline void testAllocateAlignasBigFoo(MemoryResource& resource) {
    AlignasBigFoo* p = static_cast<AlignasBigFoo*>(
      resource.allocate(sizeof(AlignasBigFoo), alignof(AlignasBigFoo)));
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(isAlign(p, alignof(AlignasBigFoo)));
    resource.deallocate(p, sizeof(AlignasBigFoo), alignof(AlignasBigFoo));
  }

  inline void testAllocateAlignasSmallFoo(MemoryResource& resource) {
    AlignasSmallFoo* p = static_cast<AlignasSmallFoo*>(
      resource.allocate(sizeof(AlignasSmallFoo), alignof(AlignasSmallFoo)));
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(isAlign(p, alignof(AlignasSmallFoo)));
    resource.deallocate(p, sizeof(AlignasSmallFoo), alignof(AlignasSmallFoo));
  }

} // namespace flow

#pragma warning(pop)
