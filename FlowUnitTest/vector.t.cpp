//#include "../DataStructure/vector.h"
//#include <gtest/gtest.h>
//
//namespace {
//  using flow::Vector;
//
//  TEST(VectorTest, DefaultCtor) {
//    Vector<int> vec;
//    EXPECT_EQ(0, vec.size());
//  }
//
//  TEST(VectorTest, CopyCtor) {
//    Vector<int> v1{ 10, 20, 30 };
//
//    {
//      Vector<int> v2(v1);
//
//      // Make sure it is a deep copy.
//      EXPECT_EQ(3, v1.size());
//      EXPECT_EQ(10, v1[0]);
//      EXPECT_EQ(20, v1[1]);
//      EXPECT_EQ(30, v1[2]);
//      EXPECT_EQ(3, v2.size());
//      EXPECT_EQ(10, v2[0]);
//      EXPECT_EQ(20, v2[1]);
//      EXPECT_EQ(30, v2[2]);
//    }
//
//    // Must not affect the original vector.
//    EXPECT_EQ(3, v1.size());
//    EXPECT_EQ(10, v1[0]);
//    EXPECT_EQ(20, v1[1]);
//    EXPECT_EQ(30, v1[2]);
//  }
//
//  TEST(VectorTest, MoveCtor) {
//    Vector<int> v1{ 10, 20, 30 };
//
//    {
//      Vector<int> v2(std::move(v1));
//
//      // Make sure it is a deep copy.
//      EXPECT_EQ(0, v1.size());
//      EXPECT_EQ(3, v2.size());
//      EXPECT_EQ(10, v2[0]);
//      EXPECT_EQ(20, v2[1]);
//      EXPECT_EQ(30, v2[2]);
//    }
//
//    // Must not affect the original vector.
//    EXPECT_EQ(0, v1.size());
//  }
//
//  TEST(VectorTest, InitializerListCtor) {
//    Vector<int> vec = { 10, 20, 30 };
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(10, vec[0]);
//    EXPECT_EQ(20, vec[1]);
//    EXPECT_EQ(30, vec[2]);
//  }
//
//  TEST(VectorTest, IteratorCtor) {
//    int arr[] = { 10, 20, 30 };
//    Vector<int> vec(arr, arr + sizeof(arr) / sizeof(int));
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(10, vec[0]);
//    EXPECT_EQ(20, vec[1]);
//    EXPECT_EQ(30, vec[2]);
//  }
//
//  TEST(VectorTest, FillCtor) {
//    Vector<int> vec(3, 5);
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(5, vec[0]);
//    EXPECT_EQ(5, vec[1]);
//    EXPECT_EQ(5, vec[2]);
//  }
//
//  TEST(VectorTest, CopyOp) {
//    Vector<int> v1 = { 10, 20, 30 };
//    Vector<int> v2 = { 40 };
//    v2 = v1;
//
//    EXPECT_EQ(3, v1.size());
//    EXPECT_EQ(10, v1[0]);
//    EXPECT_EQ(20, v1[1]);
//    EXPECT_EQ(30, v1[2]);
//
//    EXPECT_EQ(3, v2.size());
//    EXPECT_EQ(10, v2[0]);
//    EXPECT_EQ(20, v2[1]);
//    EXPECT_EQ(30, v2[2]);
//  }
//
//  TEST(VectorTest, MoveOp) {
//    Vector<int> v1 = { 10, 20 };
//    v1.push_back(30); // ensure capacity > size
//    size_t sz = v1.size();
//    size_t cap = v1.capacity();
//    ASSERT_TRUE(cap > sz);
//
//    Vector<int> v2;
//    v2 = std::move(v1);
//
//    EXPECT_EQ(0, v1.size());
//    EXPECT_EQ(0, v1.capacity());
//
//    EXPECT_EQ(sz, v2.size());
//    EXPECT_EQ(cap, v2.capacity());
//    EXPECT_EQ(10, v2[0]);
//    EXPECT_EQ(20, v2[1]);
//    EXPECT_EQ(30, v2[2]);
//  }
//
//  TEST(VectorTest, ClearAndEmpty) {
//    Vector<int> vec = { 10, 20, 30 };
//    EXPECT_TRUE(!vec.empty());
//    vec.clear();
//    EXPECT_TRUE(vec.empty());
//  }
//
//  TEST(VectorTest, PositionalAccesser) {
//    Vector<int> vec = { 10, 20, 30 };
//    EXPECT_EQ(10, vec.front());
//    EXPECT_EQ(30, vec.back());
//  }
//
//  TEST(VectorTest, IteratorOps) {
//    Vector<int> vec = { 10, 20, 30 };
//
//    auto it = vec.begin();
//    EXPECT_EQ(10, *it);
//    auto old = it++;
//    EXPECT_EQ(10, *old);
//    EXPECT_EQ(20, *it);
//    ++it;
//    EXPECT_EQ(30, *it);
//    ++it;
//    EXPECT_EQ(vec.end(), it);
//  }
//
//  TEST(VectorTest, Reserve) {
//    Vector<int> vec;
//    EXPECT_EQ(0, vec.capacity());
//    vec.reserve(100);
//    EXPECT_EQ(100, vec.capacity());
//  }
//
//  TEST(VectorTest, Resize) {
//    Vector<int> vec;
//    vec.resize(3);
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(0, vec[0]);
//    EXPECT_EQ(0, vec[1]);
//    EXPECT_EQ(0, vec[2]);
//
//    vec.resize(6, 2);
//    EXPECT_EQ(6, vec.size());
//    EXPECT_EQ(0, vec[0]);
//    EXPECT_EQ(0, vec[1]);
//    EXPECT_EQ(0, vec[2]);
//    EXPECT_EQ(2, vec[3]);
//    EXPECT_EQ(2, vec[4]);
//    EXPECT_EQ(2, vec[5]);
//  }
//
//  TEST(VectorTest, PushBackAndPopBack) {
//    Vector<std::string> vec;
//
//    vec.push_back("hello");
//    EXPECT_EQ(1, vec.size());
//    EXPECT_EQ("hello", vec[0]);
//
//    vec.push_back(3, 'a');
//    EXPECT_EQ(2, vec.size());
//    EXPECT_EQ("aaa", vec[1]);
//
//    vec.pop_back();
//    EXPECT_EQ(1, vec.size());
//    vec.pop_back();
//    EXPECT_EQ(0, vec.size());
//  }
//
//  TEST(VectorTest, EraseFrontAndBack) {
//    Vector<int> vec = { 1, 2, 3, 4, 5 };
//    vec.erase(vec.begin());
//    EXPECT_EQ(4, vec.size());
//    EXPECT_EQ(2, vec[0]);
//    EXPECT_EQ(3, vec[1]);
//    EXPECT_EQ(4, vec[2]);
//    EXPECT_EQ(5, vec[3]);
//
//    vec.erase(vec.begin() + 3);
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(2, vec[0]);
//    EXPECT_EQ(3, vec[1]);
//    EXPECT_EQ(4, vec[2]);
//  }
//
//  TEST(VectorTest, EraseRange) {
//    Vector<int> vec = { 1, 2, 3, 4, 5 };
//    vec.erase(vec.begin() + 1, vec.begin() + 3);
//
//    EXPECT_EQ(3, vec.size());
//    EXPECT_EQ(1, vec[0]);
//    EXPECT_EQ(4, vec[1]);
//    EXPECT_EQ(5, vec[2]);
//
//    vec.erase(vec.begin(), vec.end());
//    EXPECT_EQ(0, vec.size());
//  }
//
//  TEST(VectorTest, InsertRangeFrontAndBack) {
//    Vector<int> vec = { 100, 200, 300 };
//
//    // Insert range front.
//    Vector<int> values = { 1, 2, 3 };
//    vec.insert(vec.begin(), values.begin(), values.end());
//    EXPECT_EQ(6, vec.size());
//    EXPECT_EQ(1, vec[0]);
//    EXPECT_EQ(2, vec[1]);
//    EXPECT_EQ(3, vec[2]);
//    EXPECT_EQ(100, vec[3]);
//    EXPECT_EQ(200, vec[4]);
//    EXPECT_EQ(300, vec[5]);
//
//    // Insert range back.
//    values = { 4, 5, 6 };
//    vec.insert(vec.end(), values.begin(), values.end());
//    EXPECT_EQ(9, vec.size());
//    EXPECT_EQ(1, vec[0]);
//    EXPECT_EQ(2, vec[1]);
//    EXPECT_EQ(3, vec[2]);
//    EXPECT_EQ(100, vec[3]);
//    EXPECT_EQ(200, vec[4]);
//    EXPECT_EQ(300, vec[5]);
//    EXPECT_EQ(4, vec[6]);
//    EXPECT_EQ(5, vec[7]);
//    EXPECT_EQ(6, vec[8]);
//
//    // Insert range middle.
//    values = { 7, 8, 9 };
//    vec.insert(vec.begin() + 3, values.begin(), values.end());
//    EXPECT_EQ(12, vec.size());
//    EXPECT_EQ(1, vec[0]);
//    EXPECT_EQ(2, vec[1]);
//    EXPECT_EQ(3, vec[2]);
//    EXPECT_EQ(7, vec[3]);
//    EXPECT_EQ(8, vec[4]);
//    EXPECT_EQ(9, vec[5]);
//    EXPECT_EQ(100, vec[6]);
//    EXPECT_EQ(200, vec[7]);
//    EXPECT_EQ(300, vec[8]);
//    EXPECT_EQ(4, vec[9]);
//    EXPECT_EQ(5, vec[10]);
//    EXPECT_EQ(6, vec[11]);
//  }
//
//  TEST(VectorTest, InsertPointFrontAndBack) {
//    Vector<int> vec;
//    for (int i = 0; i < 32; ++i) {
//      vec.insert(vec.begin(), i);
//    }
//    for (int i = 0; i < 32; ++i) {
//      EXPECT_EQ(32 - i - 1, vec[i]);
//    }
//
//    for (int i = 0; i < 32; ++i) {
//      vec.insert(vec.end(), i);
//    }
//    for (int i = 32; i < 64; ++i) {
//      EXPECT_EQ(i - 32, vec[i]);
//    }
//  }
//  TEST(VectorTest, MapIntToString) {
//    Vector<int> vec = { 1, 2, 3 };
//    auto strVec = vec.map([](int n) {return std::string(n, 'a');  });
//    EXPECT_EQ(3, strVec.size());
//    EXPECT_EQ("a", strVec[0]);
//    EXPECT_EQ("aa", strVec[1]);
//    EXPECT_EQ("aaa", strVec[2]);
//  }
//
//  TEST(VectorTest, FilterOdd) {
//    Vector<int> vec = { 1, 2, 3, 4 };
//    auto filteredVec = vec.filter([](int n) { return n % 2 == 0;  });
//    EXPECT_EQ(2, filteredVec.size());
//    EXPECT_EQ(2, filteredVec[0]);
//    EXPECT_EQ(4, filteredVec[1]);
//  }
//
//  TEST(VectorTest, ForEach) {
//    int odd = 0;
//    const Vector<int> vec = { 1, 2, 3, 4, 5 };
//    vec.for_each([&](const int& n) {
//      if (n % 2 == 0) {
//        ++odd;
//      }
//    });
//    EXPECT_EQ(odd, 2);
//  }
//
//  TEST(VectorTest, Swap) {
//    Vector<int> v1 = { 1, 2 };
//    Vector<int> v2 = { 3 };
//    size_t v1Sz = v1.size();
//    size_t v1Cap = v1.capacity();
//    size_t v2Sz = v2.size();
//    size_t v2Cap = v2.capacity();
//
//    swap(v1, v2);
//    EXPECT_EQ(v1Sz, v2.size());
//    EXPECT_EQ(v1Cap, v2.capacity());
//    EXPECT_EQ(3, v1[0]);
//
//    EXPECT_EQ(v2Sz, v1.size());
//    EXPECT_EQ(v2Cap, v1.capacity());
//    EXPECT_EQ(1, v2[0]);
//    EXPECT_EQ(2, v2[1]);
//  }
//
//  TEST(VectorTest, EqualityOp) {
//    Vector<int> v1 = { 1, 2, 3 };
//    const Vector<int> v2 = { 1, 2 };
//    Vector<int> v3 = { 1, 2, 3 };
//    EXPECT_TRUE(v1 != v2);
//    EXPECT_FALSE(v1 == v2);
//    EXPECT_TRUE(v1 == v3);
//    EXPECT_FALSE(v1 != v3);
//  }
//
//  TEST(VectorTest, Zip) {
//    Vector<int> v1 = { 1, 2, 3 };
//    Vector<std::string> v2 = { "one", "two" };
//    Vector<flow::Tuple<int, std::string>> v3 = flow::zip(v1, v2);
//
//    EXPECT_EQ(2, v3.size());
//    EXPECT_EQ(1, v3[0].get<0>());
//    EXPECT_EQ("one", v3[0].get<1>());
//    EXPECT_EQ(2, v3[1].get<0>());
//    EXPECT_EQ("two", v3[1].get<1>());
//  }
//
//  TEST(VectorTest, Split) {
//    // Empty delimiter always return the input line
//    Vector<std::string> tokens = flow::split("", "");
//    Vector<std::string> answer{ "" };
//    EXPECT_EQ(answer, tokens);
//
//    tokens = flow::split(",", ",");
//    answer = { "", "" };
//    EXPECT_EQ(answer, tokens);
//
//    tokens = flow::split("abc", ",");
//    answer = { "abc" };
//    EXPECT_EQ(answer, tokens);
//
//    tokens = flow::split(",,123,", ",");
//    answer = { "", "", "123", "" };
//    EXPECT_EQ(answer, tokens);
//
//    tokens = flow::split("key??value", "??");
//    answer = { "key", "value" };
//    EXPECT_EQ(answer, tokens);
//  }
//
//  TEST(VectorTest, Join) {
//    Vector<std::string> tokens = { "", "", "," };
//    EXPECT_EQ(",", flow::join(tokens, ""));
//
//    tokens = { "1+1", "2", "two" };
//    EXPECT_EQ("1+1==2==two", flow::join(tokens, "=="));
//  }
//}
