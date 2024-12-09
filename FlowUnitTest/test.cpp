#include "pch.h"
#include "../DataStructure/vector.h"
#include <type_traits>

using flow::Vector;

TEST(VectorTest, DefaultCtor) {
  Vector<int> vec;
  EXPECT_EQ(0, vec.size());
}

TEST(VectorTest, CopyCtor) {
  Vector<int> v1{ 10, 20, 30 };

  {
    Vector<int> v2(v1);

    // Make sure it is a deep copy.
    EXPECT_EQ(3, v1.size());
    EXPECT_EQ(10, v1[0]);
    EXPECT_EQ(20, v1[1]);
    EXPECT_EQ(30, v1[2]);
    EXPECT_EQ(3, v2.size());
    EXPECT_EQ(10, v2[0]);
    EXPECT_EQ(20, v2[1]);
    EXPECT_EQ(30, v2[2]);
  }

  // Must not affect the original vector.
  EXPECT_EQ(3, v1.size());
  EXPECT_EQ(10, v1[0]);
  EXPECT_EQ(20, v1[1]);
  EXPECT_EQ(30, v1[2]);
}

TEST(VectorTest, MoveCtor) {
  Vector<int> v1{ 10, 20, 30 };

  {
    Vector<int> v2(std::move(v1));

    // Make sure it is a deep copy.
    EXPECT_EQ(0, v1.size());
    EXPECT_EQ(3, v2.size());
    EXPECT_EQ(10, v2[0]);
    EXPECT_EQ(20, v2[1]);
    EXPECT_EQ(30, v2[2]);
  }

  // Must not affect the original vector.
  EXPECT_EQ(0, v1.size());
}

TEST(VectorTest, InitializerListCtor) {
  Vector<int> vec = { 10, 20, 30 };
  EXPECT_EQ(3, vec.size());
  EXPECT_EQ(10, vec[0]);
  EXPECT_EQ(20, vec[1]);
  EXPECT_EQ(30, vec[2]);
}

TEST(VectorTest, IteratorCtor) {
  int arr[] = { 10, 20, 30 };
  Vector<int> vec(arr, arr + sizeof(arr) / sizeof(int));
  EXPECT_EQ(3, vec.size());
  EXPECT_EQ(10, vec[0]);
  EXPECT_EQ(20, vec[1]);
  EXPECT_EQ(30, vec[2]);
}

TEST(VectorTest, FillCtor) {
  Vector<int> vec(3, 5);
  EXPECT_EQ(3, vec.size());
  EXPECT_EQ(5, vec[0]);
  EXPECT_EQ(5, vec[1]);
  EXPECT_EQ(5, vec[2]);
}
