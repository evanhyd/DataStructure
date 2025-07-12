#include "../DataStructure/flow_vector.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <set>
#include <string>
#include <type_traits>

namespace flow {

    TEST(VectorTest, DefaultCtor) {
      {
        Vector<int> vec;
        EXPECT_EQ(0, vec.size());
        EXPECT_EQ(0, vec.capacity());
      }

      {
        Vector<std::unique_ptr<int>> vec;
        EXPECT_EQ(0, vec.size());
        EXPECT_EQ(0, vec.capacity());
      }
    }

    TEST(VectorTest, CopyCtor) {
      Vector<std::string> v1{ "ab", "cd", "ef"};

      {
        Vector<std::string> v2(v1);

        // Make sure it is a deep copy.
        EXPECT_EQ(3, v1.size());
        EXPECT_EQ("ab", v1[0]);
        EXPECT_EQ("cd", v1[1]);
        EXPECT_EQ("ef", v1[2]);

        EXPECT_EQ(3, v2.size());
        EXPECT_EQ("ab", v2[0]);
        EXPECT_EQ("cd", v2[1]);
        EXPECT_EQ("ef", v2[2]);
      }

      // Must not affect the original vector.
      EXPECT_EQ(3, v1.size());
      EXPECT_EQ("ab", v1[0]);
      EXPECT_EQ("cd", v1[1]);
      EXPECT_EQ("ef", v1[2]);
    }

    TEST(VectorTest, MoveCtor) {
      Vector<std::string> v1{ "ab", "cd", "ef"};

      {
        Vector<std::string> v2(std::move(v1));

        EXPECT_EQ(0, v1.size());
        EXPECT_EQ(0, v1.capacity());

        EXPECT_EQ(3, v2.size());
        EXPECT_EQ("ab", v2[0]);
        EXPECT_EQ("cd", v2[1]);
        EXPECT_EQ("ef", v2[2]);
      }

      // The original vector get moved.
      EXPECT_EQ(0, v1.size());
      EXPECT_EQ(0, v1.capacity());
    }

    TEST(VectorTest, IteratorCopyCtor) {
      std::set<std::string> strs = { "ab", "cd", "ef" };

      Vector<std::string> vec(std::begin(strs), std::end(strs));
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("ef", vec[2]);

      EXPECT_TRUE(strs.contains("ab"));
      EXPECT_TRUE(strs.contains("cd"));
      EXPECT_TRUE(strs.contains("ef"));
    }

    TEST(VectorTest, IteratorMoveCtor) {
      std::vector<std::string> strs = {"ab", "cd", "ef"};

      Vector<std::string> vec(std::move_iterator(std::begin(strs)), std::move_iterator(std::end(strs)));
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("ef", vec[2]);

      EXPECT_EQ(3, strs.size());
      EXPECT_EQ("", strs[0]);
      EXPECT_EQ("", strs[1]);
      EXPECT_EQ("", strs[2]);
    }

    TEST(VectorTest, InitializerListCtor) {
      Vector<std::string> vec = { "ab", "cd", "ef" };
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("ef", vec[2]);
    }
    
    TEST(VectorTest, FillCtorWithDefaultConstructedNonCopyable) {
      Vector<std::unique_ptr<int>> vec(3);
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ(nullptr, vec[0]);
      EXPECT_EQ(nullptr, vec[1]);
      EXPECT_EQ(nullptr, vec[2]);
    }

    TEST(VectorTest, FillCtorWithCopyable) {
      Vector<std::string> vec(3, "abc");
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ("abc", vec[0]);
      EXPECT_EQ("abc", vec[1]);
      EXPECT_EQ("abc", vec[2]);
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

    TEST(VectorTest, CopyOpSelf) {
      Vector<int> v1 = { 10, 20, 30 };
      v1 = v1;

      EXPECT_EQ(3, v1.size());
      EXPECT_EQ(10, v1[0]);
      EXPECT_EQ(20, v1[1]);
      EXPECT_EQ(30, v1[2]);
    }

    TEST(VectorTest, MoveOp) {
      Vector<int> v1 = { 10, 20, 30 };
      v1.reserve(5);
      size_t sz = v1.size();
      size_t cap = v1.capacity();

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

    TEST(VectorTest, MoveOpSelf) {
      Vector<int> v1 = { 10, 20, 30 };
      v1 = std::move(v1);

      EXPECT_EQ(3, v1.size());
      EXPECT_EQ(10, v1[0]);
      EXPECT_EQ(20, v1[1]);
      EXPECT_EQ(30, v1[2]);
    }

    TEST(VectorTest, ClearAndEmpty) {
      Vector<int> vec = { 10, 20, 30 };
      EXPECT_TRUE(!vec.empty());
      vec.clear();
      EXPECT_TRUE(vec.empty());
      EXPECT_EQ(0, vec.size());
      EXPECT_GT(vec.capacity(), 0);
    }

    TEST(VectorTest, SubscriptOpReadAndWrite) {
      Vector<int> vec = { 10, 20, 30 };
      EXPECT_EQ(10, vec.front());
      EXPECT_EQ(30, vec.back());
      vec.front() = 1;
      vec.back() = 3;
      EXPECT_EQ(1, vec.front());
      EXPECT_EQ(3, vec.back());
    }

    TEST(VectorTest, SubscriptOpReadOnConst) {
      const Vector<int> vec = { 10, 20, 30 };
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

    TEST(VectorTest, ConstIteratorOps) {
      const Vector<int> vec = { 10, 20, 30 };

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

    TEST(VectorTest, IteratorSTLCompatible) {
      Vector<int> vec = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
      auto it = vec.begin();
      std::advance(it, vec.size());
      EXPECT_EQ(it, vec.end());

      int sum = std::accumulate(vec.begin(), vec.end(), 0);
      EXPECT_EQ(450, sum);

      bool exist = std::binary_search(vec.begin(), vec.end(), 80);
      EXPECT_TRUE(exist);

      exist = std::binary_search(vec.begin(), vec.end(), 81);
      EXPECT_FALSE(exist);

      std::reverse(vec.begin(), vec.end());
      EXPECT_EQ(90, *vec.begin());
      EXPECT_EQ(10, *(vec.end()-1));
    }

    TEST(VectorTest, ReserveMore) {
      Vector<int> vec;
      EXPECT_EQ(0, vec.capacity());
      vec.reserve(100);
      EXPECT_EQ(100, vec.capacity());
    }

    TEST(VectorTest, ReserveLessButFail) {
      Vector<int> vec;
      EXPECT_EQ(0, vec.capacity());
      vec.reserve(100);
      EXPECT_EQ(100, vec.capacity());
      vec.reserve(1);
      EXPECT_EQ(100, vec.capacity());
    }

    TEST(VectorTest, ResizeWithValue) {
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

      vec.resize(4);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(0, vec[0]);
      EXPECT_EQ(0, vec[1]);
      EXPECT_EQ(0, vec[2]);
      EXPECT_EQ(2, vec[3]);
    }

    TEST(VectorTest, ResizeWithDefault) {
      Vector<std::unique_ptr<int>> vec;
      vec.resize(3);
      EXPECT_EQ(3, vec.size());
      EXPECT_EQ(nullptr, vec[0]);
      EXPECT_EQ(nullptr, vec[1]);
      EXPECT_EQ(nullptr, vec[2]);

      vec.resize(6);
      EXPECT_EQ(6, vec.size());
      EXPECT_EQ(nullptr, vec[0]);
      EXPECT_EQ(nullptr, vec[1]);
      EXPECT_EQ(nullptr, vec[2]);
      EXPECT_EQ(nullptr, vec[3]);
      EXPECT_EQ(nullptr, vec[4]);
      EXPECT_EQ(nullptr, vec[5]);

      vec.resize(4);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(nullptr, vec[0]);
      EXPECT_EQ(nullptr, vec[1]);
      EXPECT_EQ(nullptr, vec[2]);
      EXPECT_EQ(nullptr, vec[3]);
    }

    TEST(VectorTest, PushBackCopy) {

      std::string str[] = { "ab", "cd", "ef" };
      flow::Vector<std::string> vec;
      for (const auto& s : str) {
        vec.pushBack(s);
      }

      EXPECT_EQ(3, vec.size());
      EXPECT_GE(vec.capacity(), 3);
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("ef", vec[2]);

      EXPECT_EQ("ab", str[0]);
      EXPECT_EQ("cd", str[1]);
      EXPECT_EQ("ef", str[2]);
    }

    TEST(VectorTest, PushBackMove) {

      std::vector<std::unique_ptr<int>> ptrs;
      for (int i = 0; i < 3; ++i) {
        ptrs.emplace_back(std::make_unique<int>(i));
      }

      flow::Vector<std::unique_ptr<int>> vec;
      for (auto& s : ptrs) {
        vec.pushBack(std::move(s));
      }

      EXPECT_EQ(3, vec.size());
      EXPECT_GE(vec.capacity(), 3);
      EXPECT_EQ(0, *vec[0]);
      EXPECT_EQ(1, *vec[1]);
      EXPECT_EQ(2, *vec[2]);

      EXPECT_EQ(nullptr, ptrs[0]);
      EXPECT_EQ(nullptr, ptrs[1]);
      EXPECT_EQ(nullptr, ptrs[2]);
    }

    TEST(VectorTest, EmplaceBack) {
      flow::Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 3; ++i) {
        vec.emplaceBack(new int(i));
      }

      EXPECT_EQ(3, vec.size());
      EXPECT_GE(vec.capacity(), 3);
      EXPECT_EQ(0, *vec[0]);
      EXPECT_EQ(1, *vec[1]);
      EXPECT_EQ(2, *vec[2]);
    }

    TEST(VectorTest, PopBack) {
      int destroyedCounter = 0;
      struct Foo {
        int& counter;
        ~Foo() {
          ++counter;
        }
      };

      flow::Vector<Foo> vec;
      vec.reserve(100);
      for (int i = 0; i < 100; ++i) {
        vec.emplaceBack(destroyedCounter);
      }
      std::size_t oldCapacity = vec.capacity();
      for (int i = 0; i < 100; ++i) {
        vec.popBack();
        EXPECT_EQ(i + 1, destroyedCounter);
        EXPECT_EQ(100 - i - 1, vec.size());
        EXPECT_EQ(oldCapacity, vec.capacity());
      }
    }

    TEST(VectorTest, PushBackAndPopBack) {
      Vector<std::string> vec;
      vec.pushBack("hello");
      EXPECT_EQ(1, vec.size());
      EXPECT_EQ("hello", vec[0]);

      vec.emplaceBack(3, 'a');
      EXPECT_EQ(2, vec.size());
      EXPECT_EQ("aaa", vec[1]);

      vec.popBack();
      EXPECT_EQ(1, vec.size());
      vec.popBack();
      EXPECT_EQ(0, vec.size());
    }

    TEST(VectorTest, EraseFront) {
      Vector<std::string> vec{ "ab", "cd", "ef", "gh", "ij" };
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.begin());
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ("cd", vec[0]);
      EXPECT_EQ("ef", vec[1]);
      EXPECT_EQ("gh", vec[2]);
      EXPECT_EQ("ij", vec[3]);
    }

    TEST(VectorTest, EraseFrontMoveOnly) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.begin());
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ(1, *vec[0]);
      EXPECT_EQ(2, *vec[1]);
      EXPECT_EQ(3, *vec[2]);
      EXPECT_EQ(4, *vec[3]);
    }

    TEST(VectorTest, EraseFrontUntilEmpty) {

      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();

      for (int i = 0; i < 5; ++i) {
        vec.erase(vec.begin());
        EXPECT_EQ(5 - i - 1, vec.size());
        EXPECT_EQ(oldCapacity, vec.capacity());
      }
    }

    TEST(VectorTest, EraseBack) {
      Vector< std::string> vec{ "ab", "cd", "ef", "gh", "ij" };
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.end() - 1);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("ef", vec[2]);
      EXPECT_EQ("gh", vec[3]);
    }

    TEST(VectorTest, EraseBackMoveOnly) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.end()-1);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ(0, *vec[0]);
      EXPECT_EQ(1, *vec[1]);
      EXPECT_EQ(2, *vec[2]);
      EXPECT_EQ(3, *vec[3]);
    }

    TEST(VectorTest, EraseMiddle) {
      Vector<std::string> vec{ "ab", "cd", "ef", "gh", "ij" };
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.begin() + 2);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ("ab", vec[0]);
      EXPECT_EQ("cd", vec[1]);
      EXPECT_EQ("gh", vec[2]);
      EXPECT_EQ("ij", vec[3]);
    }

    TEST(VectorTest, EraseMidMoveOnly) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.begin() + 2);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ(0, *vec[0]);
      EXPECT_EQ(1, *vec[1]);
      EXPECT_EQ(3, *vec[2]);
      EXPECT_EQ(4, *vec[3]);
    }

    TEST(VectorTest, EraseRangeByOne) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();

      vec.erase(vec.begin(), vec.begin() + 1);
      EXPECT_EQ(4, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ(1, *vec[0]);
      EXPECT_EQ(2, *vec[1]);
      EXPECT_EQ(3, *vec[2]);
      EXPECT_EQ(4, *vec[3]);
    }

    TEST(VectorTest, EraseRangeAll) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();
      vec.erase(vec.begin(), vec.end());
      EXPECT_EQ(0, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
    }

    TEST(VectorTest, EraseRangeInMiddle) {
      Vector<std::unique_ptr<int>> vec;
      for (int i = 0; i < 5; ++i) {
        vec.pushBack(std::make_unique<int>(i));
      }
      std::size_t oldCapacity = vec.capacity();
      vec.erase(vec.begin() + 1, vec.begin() + 4);
      EXPECT_EQ(2, vec.size());
      EXPECT_EQ(oldCapacity, vec.capacity());
      EXPECT_EQ(0, *vec[0]);
      EXPECT_EQ(4, *vec[1]);
    }

    TEST(VectorTest, InsertMultipleLValues) {
      Vector<std::string> v;

      // Insert at end initially
      std::string val1 = "first";
      v.insert(v.end(), val1);
      ASSERT_EQ(v.size(), 1);
      EXPECT_EQ(v[0], "first");

      // Insert at beginning
      std::string val2 = "zero";
      v.insert(v.begin(), val2);
      ASSERT_EQ(v.size(), 2);
      EXPECT_EQ(v[0], "zero");
      EXPECT_EQ(v[1], "first");

      // Insert in the middle
      std::string val3 = "middle";
      v.insert(v.begin() + 1, val3);
      ASSERT_EQ(v.size(), 3);
      EXPECT_EQ(v[0], "zero");
      EXPECT_EQ(v[1], "middle");
      EXPECT_EQ(v[2], "first");

      // Insert at end again
      std::string val4 = "last";
      v.insert(v.end(), val4);
      ASSERT_EQ(v.size(), 4);
      EXPECT_EQ(v[0], "zero");
      EXPECT_EQ(v[1], "middle");
      EXPECT_EQ(v[2], "first");
      EXPECT_EQ(v[3], "last");
    }

    TEST(VectorTest, InsertMultipleRValues) {
      Vector<std::unique_ptr<int>> v;

      // Insert at end initially
      v.insert(v.end(), std::make_unique<int>(10));
      ASSERT_EQ(v.size(), 1);
      EXPECT_EQ(*v[0], 10);

      // Insert at beginning
      v.insert(v.begin(), std::make_unique<int>(5));
      ASSERT_EQ(v.size(), 2);
      EXPECT_EQ(*v[0], 5);
      EXPECT_EQ(*v[1], 10);

      // Insert in the middle
      v.insert(v.begin() + 1, std::make_unique<int>(7));
      ASSERT_EQ(v.size(), 3);
      EXPECT_EQ(*v[0], 5);
      EXPECT_EQ(*v[1], 7);
      EXPECT_EQ(*v[2], 10);

      // Insert at end again
      v.insert(v.end(), std::make_unique<int>(20));
      ASSERT_EQ(v.size(), 4);
      EXPECT_EQ(*v[0], 5);
      EXPECT_EQ(*v[1], 7);
      EXPECT_EQ(*v[2], 10);
      EXPECT_EQ(*v[3], 20);
    }

    TEST(VectorTest, InsertAtBeginning_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("one"));  // [one, one, one, one, one]

      v.insert(v.begin(), 2, std::string("nine"));  // [nine, nine, one, one, one, one, one]

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "nine");
      EXPECT_EQ(v[1], "nine");
      EXPECT_EQ(v[2], "one");
      EXPECT_EQ(v[3], "one");
      EXPECT_EQ(v[4], "one");
      EXPECT_EQ(v[5], "one");
      EXPECT_EQ(v[6], "one");
    }

    TEST(VectorTest, InsertAtBeginning_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("one"));  // [one x5]

      v.insert(v.begin(), 3, std::string("two"));  // reallocation

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "two");
      EXPECT_EQ(v[1], "two");
      EXPECT_EQ(v[2], "two");
      EXPECT_EQ(v[3], "one");
      EXPECT_EQ(v[4], "one");
      EXPECT_EQ(v[5], "one");
      EXPECT_EQ(v[6], "one");
      EXPECT_EQ(v[7], "one");
    }

    TEST(VectorTest, InsertAtBeginning_PartialFitTriggersReallocation) {
      Vector<std::string> v;
      v.reserve(6);
      v.insert(v.end(), 5, std::string("x"));  // size = 5, cap = 6

      // Only 1 more element would fit
      v.insert(v.begin(), 3, std::string("begin"));

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "begin");
      EXPECT_EQ(v[1], "begin");
      EXPECT_EQ(v[2], "begin");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
    }

    TEST(VectorTest, InsertInMiddle_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 6, std::string("one"));  // [one x6]

      v.insert(v.begin() + 3, 2, std::string("five"));  // insert at index 3

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "one");
      EXPECT_EQ(v[1], "one");
      EXPECT_EQ(v[2], "one");
      EXPECT_EQ(v[3], "five");
      EXPECT_EQ(v[4], "five");
      EXPECT_EQ(v[5], "one");
      EXPECT_EQ(v[6], "one");
      EXPECT_EQ(v[7], "one");
    }

    TEST(VectorTest, InsertInMiddle_WithReallocation) {
      Vector<std::string> v;
      v.reserve(6);
      v.insert(v.end(), 6, std::string("one"));  // [one x6]

      v.insert(v.begin() + 3, 4, std::string("seven"));  // forces reallocation

      ASSERT_EQ(v.size(), 10);
      EXPECT_EQ(v[0], "one");
      EXPECT_EQ(v[1], "one");
      EXPECT_EQ(v[2], "one");
      EXPECT_EQ(v[3], "seven");
      EXPECT_EQ(v[4], "seven");
      EXPECT_EQ(v[5], "seven");
      EXPECT_EQ(v[6], "seven");
      EXPECT_EQ(v[7], "one");
      EXPECT_EQ(v[8], "one");
      EXPECT_EQ(v[9], "one");
    }

    TEST(VectorTest, InsertInMiddle_PartialFitTriggersReallocation) {
      Vector<std::string> v;
      v.reserve(7);
      v.insert(v.end(), 6, std::string("m"));  // size = 6, cap = 7

      // Try inserting 3 at index 2, only 1 fits
      v.insert(v.begin() + 2, 3, std::string("middle"));

      ASSERT_EQ(v.size(), 9);
      EXPECT_EQ(v[0], "m");
      EXPECT_EQ(v[1], "m");
      EXPECT_EQ(v[2], "middle");
      EXPECT_EQ(v[3], "middle");
      EXPECT_EQ(v[4], "middle");
      EXPECT_EQ(v[5], "m");
      EXPECT_EQ(v[6], "m");
      EXPECT_EQ(v[7], "m");
      EXPECT_EQ(v[8], "m");
    }

    TEST(VectorTest, InsertAtEnd_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("one"));  // [one x5]

      v.insert(v.end(), 2, std::string("nine"));  // [one x5, nine x2]

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "one");
      EXPECT_EQ(v[1], "one");
      EXPECT_EQ(v[2], "one");
      EXPECT_EQ(v[3], "one");
      EXPECT_EQ(v[4], "one");
      EXPECT_EQ(v[5], "nine");
      EXPECT_EQ(v[6], "nine");
    }

    TEST(VectorTest, InsertAtEnd_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("one"));  // [one x5]

      v.insert(v.end(), 5, std::string("eight"));  // triggers reallocation

      ASSERT_EQ(v.size(), 10);
      EXPECT_EQ(v[0], "one");
      EXPECT_EQ(v[1], "one");
      EXPECT_EQ(v[2], "one");
      EXPECT_EQ(v[3], "one");
      EXPECT_EQ(v[4], "one");
      EXPECT_EQ(v[5], "eight");
      EXPECT_EQ(v[6], "eight");
      EXPECT_EQ(v[7], "eight");
      EXPECT_EQ(v[8], "eight");
      EXPECT_EQ(v[9], "eight");
    }

    TEST(VectorTest, InsertAtEnd_PartialFitTriggersReallocation) {
      Vector<std::string> v;
      v.reserve(6);
      v.insert(v.end(), 5, std::string("end"));  // size = 5, cap = 6

      // Try inserting 3 more - only 1 fits.
      v.insert(v.end(), 3, std::string("append"));

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "end");
      EXPECT_EQ(v[1], "end");
      EXPECT_EQ(v[2], "end");
      EXPECT_EQ(v[3], "end");
      EXPECT_EQ(v[4], "end");
      EXPECT_EQ(v[5], "append");
      EXPECT_EQ(v[6], "append");
      EXPECT_EQ(v[7], "append");
    }

    TEST(VectorTest, InsertIteratorRangeAtBeginning_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("x"));

      std::set<std::string> input = { "a", "b" };

      v.insert(v.begin(), input.begin(), input.end());

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "a");
      EXPECT_EQ(v[1], "b");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
    }

    TEST(VectorTest, InsertIteratorRangeAtBeginning_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("x"));

      std::set<std::string> input = { "a", "b", "c" };

      v.insert(v.begin(), input.begin(), input.end());

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "a");
      EXPECT_EQ(v[1], "b");
      EXPECT_EQ(v[2], "c");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
    }

    TEST(VectorTest, InsertIteratorRangeInMiddle_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 6, std::string("x"));

      std::set<std::string> input = { "m1", "m2" };

      v.insert(v.begin() + 3, input.begin(), input.end());

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "m1");
      EXPECT_EQ(v[4], "m2");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
    }

    TEST(VectorTest, InsertIteratorRangeInMiddle_WithReallocation) {
      Vector<std::string> v;
      v.reserve(6);
      v.insert(v.end(), 6, std::string("x"));

      std::set<std::string> input = { "m1", "m2", "m3" };

      v.insert(v.begin() + 2, input.begin(), input.end());

      ASSERT_EQ(v.size(), 9);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "m1");
      EXPECT_EQ(v[3], "m2");
      EXPECT_EQ(v[4], "m3");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
      EXPECT_EQ(v[8], "x");
    }

    TEST(VectorTest, InsertIteratorRangeAtEnd_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("x"));

      std::set<std::string> input = { "y1", "y2" };

      v.insert(v.end(), input.begin(), input.end());

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "y1");
      EXPECT_EQ(v[6], "y2");
    }

    TEST(VectorTest, InsertIteratorRangeAtEnd_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("x"));

      std::set<std::string> input = { "y1", "y2", "y3" };

      v.insert(v.end(), input.begin(), input.end());

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "y1");
      EXPECT_EQ(v[6], "y2");
      EXPECT_EQ(v[7], "y3");
    }

    TEST(VectorTest, InsertInitializerListAtBeginning_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("x"));

      v.insert(v.begin(), { "a", "b" });

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "a");
      EXPECT_EQ(v[1], "b");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
    }

    TEST(VectorTest, InsertInitializerListAtBeginning_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("x"));

      v.insert(v.begin(), { "a", "b", "c" });

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "a");
      EXPECT_EQ(v[1], "b");
      EXPECT_EQ(v[2], "c");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
    }

    TEST(VectorTest, InsertInitializerListInMiddle_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 6, std::string("x"));

      v.insert(v.begin() + 3, { "m1", "m2" });

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "m1");
      EXPECT_EQ(v[4], "m2");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
    }

    TEST(VectorTest, InsertInitializerListInMiddle_WithReallocation) {
      Vector<std::string> v;
      v.reserve(6);
      v.insert(v.end(), 6, std::string("x"));

      v.insert(v.begin() + 2, { "m1", "m2", "m3" });

      ASSERT_EQ(v.size(), 9);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "m1");
      EXPECT_EQ(v[3], "m2");
      EXPECT_EQ(v[4], "m3");
      EXPECT_EQ(v[5], "x");
      EXPECT_EQ(v[6], "x");
      EXPECT_EQ(v[7], "x");
      EXPECT_EQ(v[8], "x");
    }

    TEST(VectorTest, InsertInitializerListAtEnd_NoReallocation) {
      Vector<std::string> v;
      v.reserve(10);
      v.insert(v.end(), 5, std::string("x"));

      v.insert(v.end(), { "y1", "y2" });

      ASSERT_EQ(v.size(), 7);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "y1");
      EXPECT_EQ(v[6], "y2");
    }

    TEST(VectorTest, InsertInitializerListAtEnd_WithReallocation) {
      Vector<std::string> v;
      v.reserve(5);
      v.insert(v.end(), 5, std::string("x"));

      v.insert(v.end(), { "y1", "y2", "y3" });

      ASSERT_EQ(v.size(), 8);
      EXPECT_EQ(v[0], "x");
      EXPECT_EQ(v[1], "x");
      EXPECT_EQ(v[2], "x");
      EXPECT_EQ(v[3], "x");
      EXPECT_EQ(v[4], "x");
      EXPECT_EQ(v[5], "y1");
      EXPECT_EQ(v[6], "y2");
      EXPECT_EQ(v[7], "y3");
    }

    TEST(VectorTest, SwapTwoNonEmptyVectors) {
      Vector<std::string> a;
      Vector<std::string> b;
      a.insert(a.end(), 3, std::string("a"));  // [a, a, a]
      b.insert(b.end(), 2, std::string("b"));  // [b, b]

      swap(a, b);

      ASSERT_EQ(a.size(), 2);
      EXPECT_EQ(a[0], "b");
      EXPECT_EQ(a[1], "b");

      ASSERT_EQ(b.size(), 3);
      EXPECT_EQ(b[0], "a");
      EXPECT_EQ(b[1], "a");
      EXPECT_EQ(b[2], "a");
    }

    TEST(VectorTest, SwapEmptyLeftVector) {
      Vector<std::string> a;  // empty
      Vector<std::string> b;
      b.insert(b.end(), 2, std::string("b"));  // [b, b]

      swap(a, b);

      ASSERT_EQ(a.size(), 2);
      EXPECT_EQ(a[0], "b");
      EXPECT_EQ(a[1], "b");

      ASSERT_EQ(b.size(), 0);
    }

    TEST(VectorTest, SwapEmptyRightVector) {
      Vector<std::string> a;
      a.insert(a.end(), 2, std::string("a"));  // [a, a]
      Vector<std::string> b;  // empty

      swap(a, b);

      ASSERT_EQ(b.size(), 2);
      EXPECT_EQ(b[0], "a");
      EXPECT_EQ(b[1], "a");

      ASSERT_EQ(a.size(), 0);
    }

    TEST(VectorTest, SwapTwoEmptyVectors) {
      Vector<std::string> a;
      Vector<std::string> b;

      swap(a, b);

      ASSERT_EQ(a.size(), 0);
      ASSERT_EQ(b.size(), 0);
    }

    TEST(VectorTest, SwapUniquePtr_NonEmptyVectors) {
      Vector<std::unique_ptr<int>> a;
      Vector<std::unique_ptr<int>> b;

      a.insert(a.end(), std::make_unique<int>(1));
      a.insert(a.end(), std::make_unique<int>(2));
      b.insert(b.end(), std::make_unique<int>(3));

      swap(a, b);

      ASSERT_EQ(a.size(), 1);
      ASSERT_EQ(*a[0], 3);

      ASSERT_EQ(b.size(), 2);
      EXPECT_EQ(*b[0], 1);
      EXPECT_EQ(*b[1], 2);
    }

    TEST(VectorTest, SwapUniquePtr_LeftEmpty) {
      Vector<std::unique_ptr<int>> a;
      Vector<std::unique_ptr<int>> b;

      b.insert(b.end(), std::make_unique<int>(10));
      b.insert(b.end(), std::make_unique<int>(20));

      swap(a, b);

      ASSERT_EQ(a.size(), 2);
      EXPECT_EQ(*a[0], 10);
      EXPECT_EQ(*a[1], 20);

      ASSERT_EQ(b.size(), 0);
    }

    TEST(VectorTest, SwapUniquePtr_RightEmpty) {
      Vector<std::unique_ptr<int>> a;
      Vector<std::unique_ptr<int>> b;

      a.insert(a.end(), std::make_unique<int>(100));

      swap(a, b);

      ASSERT_EQ(b.size(), 1);
      EXPECT_EQ(*b[0], 100);

      ASSERT_EQ(a.size(), 0);
    }

    TEST(VectorTest, SwapUniquePtr_TwoEmptyVectors) {
      Vector<std::unique_ptr<int>> a;
      Vector<std::unique_ptr<int>> b;

      swap(a, b);

      ASSERT_EQ(a.size(), 0);
      ASSERT_EQ(b.size(), 0);
    }

    TEST(VectorTest, EqualityOperator_SameElements) {
      Vector<std::string> a;
      Vector<std::string> b;

      a.insert(a.end(), 3, std::string("same"));
      b.insert(b.end(), 3, std::string("same"));

      EXPECT_TRUE(a == b);
      EXPECT_FALSE(a != b);
    }

    TEST(VectorTest, EqualityOperator_DifferentElements) {
      Vector<std::string> a;
      Vector<std::string> b;

      a.insert(a.end(), 3, std::string("a"));
      b.insert(b.end(), 3, std::string("b"));

      EXPECT_FALSE(a == b);
      EXPECT_TRUE(a != b);
    }

    TEST(VectorTest, EqualityOperator_DifferentSizes) {
      Vector<std::string> a;
      Vector<std::string> b;

      a.insert(a.end(), { "x", "y" });
      b.insert(b.end(), { "x", "y", "z" });

      EXPECT_FALSE(a == b);
      EXPECT_TRUE(a != b);
    }

    TEST(VectorTest, EqualityOperator_BothEmpty) {
      Vector<std::string> a;
      Vector<std::string> b;

      EXPECT_TRUE(a == b);
      EXPECT_FALSE(a != b);
    }

    TEST(VectorTest, EqualityOperator_OneEmpty) {
      Vector<std::string> a;
      Vector<std::string> b;

      b.insert(b.end(), { "not", "empty" });

      EXPECT_FALSE(a == b);
      EXPECT_TRUE(a != b);
    }
}

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
