#include <gtest/gtest.h>
#include "../DataStructure/counted_value_view_iterator.h"

TEST(CountedValueViewIteratorTest, DereferenceAndArrowInt) {
  int value = 42;
  flow::CountedValueViewIterator<int> it(value, 3);

  EXPECT_EQ(*it, 42);
  EXPECT_EQ(it.operator->(), &value);
}

TEST(CountedValueViewIteratorTest, DereferenceAndArrowString) {
  std::string value = "hello";
  flow::CountedValueViewIterator<std::string> it(value, 2);

  EXPECT_EQ(*it, "hello");
  EXPECT_EQ(it.operator->(), &value);
}

TEST(CountedValueViewIteratorTest, EqualityAndInequalityInt) {
  int value = 7;
  flow::CountedValueViewIterator<int> it1(value, 3);
  flow::CountedValueViewIterator<int> it2(value, 3);
  flow::CountedValueViewIterator<int> it3(value, 0);

  EXPECT_TRUE(it1 == it2);
  EXPECT_FALSE(it1 != it2);
  EXPECT_FALSE(it1 == it3);
  EXPECT_TRUE(it1 != it3);
}

TEST(CountedValueViewIteratorTest, EqualityAndInequalityString) {
  std::string value = "world";
  flow::CountedValueViewIterator<std::string> it1(value, 4);
  flow::CountedValueViewIterator<std::string> it2(value, 4);
  flow::CountedValueViewIterator<std::string> it3(value, 1);

  EXPECT_TRUE(it1 == it2);
  EXPECT_FALSE(it1 != it2);
  EXPECT_FALSE(it1 == it3);
  EXPECT_TRUE(it1 != it3);
}

TEST(CountedValueViewIteratorTest, PreIncrementInt) {
  int value = 10;
  flow::CountedValueViewIterator<int> it(value, 2);
  flow::CountedValueViewIterator<int> end(value, 0);

  ++it;
  EXPECT_EQ(it, flow::CountedValueViewIterator<int>(value, 1));
  ++it;
  EXPECT_EQ(it, end);
}

TEST(CountedValueViewIteratorTest, PreIncrementString) {
  std::string value = "test";
  flow::CountedValueViewIterator<std::string> it(value, 2);
  flow::CountedValueViewIterator<std::string> end(value, 0);

  ++it;
  EXPECT_EQ(it, flow::CountedValueViewIterator<std::string>(value, 1));
  ++it;
  EXPECT_EQ(it, end);
}

TEST(CountedValueViewIteratorTest, PostIncrementInt) {
  int value = 15;
  flow::CountedValueViewIterator<int> it(value, 2);
  flow::CountedValueViewIterator<int> end(value, 0);

  auto old = it++;
  EXPECT_EQ(old, flow::CountedValueViewIterator<int>(value, 2));
  EXPECT_EQ(it, flow::CountedValueViewIterator<int>(value, 1));

  old = it++;
  EXPECT_EQ(old, flow::CountedValueViewIterator<int>(value, 1));
  EXPECT_EQ(it, end);
}

TEST(CountedValueViewIteratorTest, PostIncrementString) {
  std::string value = "post";
  flow::CountedValueViewIterator<std::string> it(value, 2);
  flow::CountedValueViewIterator<std::string> end(value, 0);

  auto old = it++;
  EXPECT_EQ(old, flow::CountedValueViewIterator<std::string>(value, 2));
  EXPECT_EQ(it, flow::CountedValueViewIterator<std::string>(value, 1));

  old = it++;
  EXPECT_EQ(old, flow::CountedValueViewIterator<std::string>(value, 1));
  EXPECT_EQ(it, end);
}

TEST(CountedValueViewIteratorTest, IterateInLoopInt) {
  int value = 5;
  flow::CountedValueViewIterator<int> begin(value, 3);
  flow::CountedValueViewIterator<int> end(value, 0);

  int count = 0;
  for (auto it = begin; it != end; ++it) {
    EXPECT_EQ(*it, 5);
    ++count;
  }
  EXPECT_EQ(count, 3);
}

TEST(CountedValueViewIteratorTest, IterateInLoopString) {
  std::string value = "loop";
  flow::CountedValueViewIterator<std::string> begin(value, 4);
  flow::CountedValueViewIterator<std::string> end(value, 0);

  int count = 0;
  for (auto it = begin; it != end; ++it) {
    EXPECT_EQ(*it, "loop");
    ++count;
  }
  EXPECT_EQ(count, 4);
}
