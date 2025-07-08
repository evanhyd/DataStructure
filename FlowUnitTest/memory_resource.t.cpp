#pragma warning(push)
#pragma warning(disable : 4324) // Disable alignas warning.
#include "../DataStructure/default_memory_resource.h"
#include "../DataStructure/memory_resource.h"
#include <gtest/gtest.h>

namespace flow {
  template <typename MemoryResourceType>
  class CommonMemoryResourceTest : public ::testing::Test {
  protected:
    MemoryResourceType resource{};

    template <typename U>
    bool isAlign(U* address, size_t alignment = alignof(U)) const {
      return reinterpret_cast<std::uintptr_t>(address) % alignment == 0;
    }

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
  };

  TYPED_TEST_CASE_P(CommonMemoryResourceTest);

  // ---------- int tests ----------

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateInt) {
    int* p = static_cast<int*>(this->resource.allocate(sizeof(int), alignof(int)));
    EXPECT_NE(p, nullptr);
    EXPECT_TRUE(this->isAlign(p));
    this->resource.deallocate(p, sizeof(int), alignof(int));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateIntDistinct) {
    int* p1 = static_cast<int*>(this->resource.allocate(sizeof(int), alignof(int)));
    int* p2 = static_cast<int*>(this->resource.allocate(sizeof(int), alignof(int)));
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(this->isAlign(p1));
    this->resource.deallocate(p1, sizeof(int), alignof(int));
    this->resource.deallocate(p2, sizeof(int), alignof(int));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateIntArray) {
    constexpr size_t kCount = 5;
    int* arr = static_cast<int*>(this->resource.allocate(sizeof(int) * kCount, alignof(int)));
    EXPECT_NE(arr, nullptr);
    for (size_t i = 0; i < kCount; ++i) {
      arr[i] = static_cast<int>(i * 2);
    }
    for (size_t i = 0; i < kCount; ++i) {
      EXPECT_EQ(arr[i], static_cast<int>(i * 2));
    }
    EXPECT_TRUE(this->isAlign(arr));
    this->resource.deallocate(arr, sizeof(int) * kCount, alignof(int));
  }

  // ---------- Foo tests ----------

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateFoo) {
    typename TestFixture::Foo* p =
      static_cast<typename TestFixture::Foo*>(this->resource.allocate(sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo)));
    EXPECT_NE(p, nullptr);
    EXPECT_TRUE(this->isAlign(p));
    this->resource.deallocate(p, sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateFooDistinct) {
    typename TestFixture::Foo* p1 =
      static_cast<typename TestFixture::Foo*>(this->resource.allocate(sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo)));
    typename TestFixture::Foo* p2 =
      static_cast<typename TestFixture::Foo*>(this->resource.allocate(sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo)));
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(this->isAlign(p1));
    this->resource.deallocate(p1, sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo));
    this->resource.deallocate(p2, sizeof(typename TestFixture::Foo), alignof(typename TestFixture::Foo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateFooArray) {
    constexpr size_t kCount = 5;
    typename TestFixture::Foo* arr =
      static_cast<typename TestFixture::Foo*>(this->resource.allocate(sizeof(typename TestFixture::Foo) * kCount, alignof(typename TestFixture::Foo)));
    EXPECT_NE(arr, nullptr);
    for (size_t i = 0; i < kCount; ++i) {
      arr[i].b = static_cast<char>(i + 1);
      arr[i].c = static_cast<int64_t>(i * 100);
    }
    for (size_t i = 0; i < kCount; ++i) {
      EXPECT_EQ(arr[i].b, static_cast<char>(i + 1));
      EXPECT_EQ(arr[i].c, static_cast<int64_t>(i * 100));
    }
    EXPECT_TRUE(this->isAlign(arr));
    this->resource.deallocate(arr, sizeof(typename TestFixture::Foo) * kCount, alignof(typename TestFixture::Foo));
  }

  // ---------- AlignasBigFoo tests ----------

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasBigFoo) {
    typename TestFixture::AlignasBigFoo* p =
      static_cast<typename TestFixture::AlignasBigFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo)));
    EXPECT_NE(p, nullptr);
    EXPECT_TRUE(this->isAlign(p, alignof(typename TestFixture::AlignasBigFoo)));
    this->resource.deallocate(p, sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasBigFooDistinct) {
    typename TestFixture::AlignasBigFoo* p1 =
      static_cast<typename TestFixture::AlignasBigFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo)));
    typename TestFixture::AlignasBigFoo* p2 =
      static_cast<typename TestFixture::AlignasBigFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo)));
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(this->isAlign(p1, alignof(typename TestFixture::AlignasBigFoo)));
    this->resource.deallocate(p1, sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo));
    this->resource.deallocate(p2, sizeof(typename TestFixture::AlignasBigFoo), alignof(typename TestFixture::AlignasBigFoo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasBigFooArray) {
    constexpr size_t kCount = 5;
    typename TestFixture::AlignasBigFoo* arr =
      static_cast<typename TestFixture::AlignasBigFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasBigFoo) * kCount, alignof(typename TestFixture::AlignasBigFoo)));
    EXPECT_NE(arr, nullptr);
    for (size_t i = 0; i < kCount; ++i) {
      arr[i].b = static_cast<char>(i + 1);
      arr[i].c = static_cast<int64_t>(i * 300);
    }
    for (size_t i = 0; i < kCount; ++i) {
      EXPECT_EQ(arr[i].b, static_cast<char>(i + 1));
      EXPECT_EQ(arr[i].c, static_cast<int64_t>(i * 300));
    }
    EXPECT_TRUE(this->isAlign(arr, alignof(typename TestFixture::AlignasBigFoo)));
    this->resource.deallocate(arr, sizeof(typename TestFixture::AlignasBigFoo) * kCount, alignof(typename TestFixture::AlignasBigFoo));
  }

  // ---------- AlignasSmallFoo tests ----------

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasSmallFoo) {
    typename TestFixture::AlignasSmallFoo* p =
      static_cast<typename TestFixture::AlignasSmallFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo)));
    EXPECT_NE(p, nullptr);
    EXPECT_TRUE(this->isAlign(p, alignof(typename TestFixture::AlignasSmallFoo)));
    this->resource.deallocate(p, sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasSmallFooDistinct) {
    typename TestFixture::AlignasSmallFoo* p1 =
      static_cast<typename TestFixture::AlignasSmallFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo)));
    typename TestFixture::AlignasSmallFoo* p2 =
      static_cast<typename TestFixture::AlignasSmallFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo)));
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_NE(p1, p2);
    EXPECT_TRUE(this->isAlign(p1, alignof(typename TestFixture::AlignasSmallFoo)));
    this->resource.deallocate(p1, sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo));
    this->resource.deallocate(p2, sizeof(typename TestFixture::AlignasSmallFoo), alignof(typename TestFixture::AlignasSmallFoo));
  }

  TYPED_TEST_P(CommonMemoryResourceTest, AllocateAlignasSmallFooArray) {
    constexpr size_t kCount = 5;
    typename TestFixture::AlignasSmallFoo* arr =
      static_cast<typename TestFixture::AlignasSmallFoo*>(this->resource.allocate(sizeof(typename TestFixture::AlignasSmallFoo) * kCount, alignof(typename TestFixture::AlignasSmallFoo)));
    EXPECT_NE(arr, nullptr);
    for (size_t i = 0; i < kCount; ++i) {
      arr[i].a = static_cast<char>(i + 1);
      arr[i].b = static_cast<int>(i * 400);
    }
    for (size_t i = 0; i < kCount; ++i) {
      EXPECT_EQ(arr[i].a, static_cast<char>(i + 1));
      EXPECT_EQ(arr[i].b, static_cast<int>(i * 400));
    }
    EXPECT_TRUE(this->isAlign(arr, alignof(typename TestFixture::AlignasSmallFoo)));
    this->resource.deallocate(arr, sizeof(typename TestFixture::AlignasSmallFoo) * kCount, alignof(typename TestFixture::AlignasSmallFoo));
  }

  // ---------- Register tests ----------

  REGISTER_TYPED_TEST_CASE_P(CommonMemoryResourceTest,
                             AllocateInt,
                             AllocateIntDistinct,
                             AllocateIntArray,

                             AllocateFoo,
                             AllocateFooDistinct,
                             AllocateFooArray,

                             AllocateAlignasBigFoo,
                             AllocateAlignasBigFooDistinct,
                             AllocateAlignasBigFooArray,

                             AllocateAlignasSmallFoo,
                             AllocateAlignasSmallFooDistinct,
                             AllocateAlignasSmallFooArray);

  using MemoryResourceTypes = ::testing::Types<flow::DefaultMemoryResource>;

  INSTANTIATE_TYPED_TEST_CASE_P(MemoryResourceTests, CommonMemoryResourceTest, MemoryResourceTypes);
}

#pragma warning(pop)
