#include "gtest/gtest.h"
#include "../DataStructure/vector.h"

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

TEST(VectorTest, CopyOp) {
  Vector<int> v1 = { 10, 20, 30 };
  Vector<int> v2 = { 40 };
  v2 = v1;

  EXPECT_EQ(3, v1.size());
  EXPECT_EQ(10, v1[0]);
  EXPECT_EQ(20, v1[1]);
  EXPECT_EQ(30, v1[2]);

  EXPECT_EQ(3, v2.size());
  EXPECT_EQ(10, v2[0]);
  EXPECT_EQ(20, v2[1]);
  EXPECT_EQ(30, v2[2]);
}

TEST(VectorTest, MoveOp) {
  Vector<int> v1 = { 10, 20 };
  v1.push_back(30); // ensure capacity > size
  size_t sz = v1.size();
  size_t cap = v1.capacity();
  ASSERT_TRUE(cap > sz);

  Vector<int> v2;
  v2 = std::move(v1);

  EXPECT_EQ(0, v1.size());
  EXPECT_EQ(0, v1.capacity());

  EXPECT_EQ(sz, v2.size());
  EXPECT_EQ(cap, v2.capacity());
  EXPECT_EQ(10, v2[0]);
  EXPECT_EQ(20, v2[1]);
  EXPECT_EQ(30, v2[2]);
}

TEST(VectorTest, ClearAndEmpty) {
  Vector<int> vec = { 10, 20, 30 };
  EXPECT_TRUE(!vec.empty());
  vec.clear();
  EXPECT_TRUE(vec.empty());
}

TEST(VectorTest, PositionalAccesser) {
  Vector<int> vec = { 10, 20, 30 };
  EXPECT_EQ(10, vec.front());
  EXPECT_EQ(30, vec.back());
}

TEST(VectorTest, IteratorOps) {
  Vector<int> vec = { 10, 20, 30 };

  auto it = vec.begin();
  EXPECT_EQ(10, *it);
  auto old = it++;
  EXPECT_EQ(10, *old);
  EXPECT_EQ(20, *it);
  ++it;
  EXPECT_EQ(30, *it);
  ++it;
  EXPECT_EQ(vec.end(), it);
}

TEST(VectorTest, Reserve) {
  Vector<int> vec;
  EXPECT_EQ(0, vec.capacity());
  vec.reserve(100);
  EXPECT_EQ(100, vec.capacity());
}

TEST(VectorTest, Resize) {
  Vector<int> vec;
  vec.resize(3);
  EXPECT_EQ(3, vec.size());
  EXPECT_EQ(0, vec[0]);
  EXPECT_EQ(0, vec[1]);
  EXPECT_EQ(0, vec[2]);

  vec.resize(6, 2);
  EXPECT_EQ(6, vec.size());
  EXPECT_EQ(0, vec[0]);
  EXPECT_EQ(0, vec[1]);
  EXPECT_EQ(0, vec[2]);
  EXPECT_EQ(2, vec[3]);
  EXPECT_EQ(2, vec[4]);
  EXPECT_EQ(2, vec[5]);
}

TEST(VectorTest, PushBackAndPopBack) {
  Vector<std::string> vec;
  EXPECT_EQ(0, vec.size());

  vec.push_back("hello");
  EXPECT_EQ(1, vec.size());
  EXPECT_EQ("hello", vec[0]);

  vec.push_back(3, 'a');
  EXPECT_EQ(2, vec.size());
  EXPECT_EQ("aaa", vec[1]);

  vec.pop_back();
  EXPECT_EQ(1, vec.size());
  vec.pop_back();
  EXPECT_EQ(0, vec.size());
}
