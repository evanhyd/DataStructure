#include "../DataStructure/flow_integral_iterator.h"
#include <gtest/gtest.h>

namespace flow {
  TEST(IntegralIteratorTest, DereferenceAndIncrement) {
    IntegralIterator it(5);
    EXPECT_EQ(*it, 5);

    ++it;
    EXPECT_EQ(*it, 6);

    it++;
    EXPECT_EQ(*it, 7);
  }

  TEST(IntegralIteratorTest, Decrement) {
    IntegralIterator<int> it(10);
    --it;
    EXPECT_EQ(*it, 9);

    it--;
    EXPECT_EQ(*it, 8);
  }

  TEST(IntegralIteratorTest, ArithmeticOperators) {
    IntegralIterator<int> it(0);

    it += 5;
    EXPECT_EQ(*it, 5);

    it -= 3;
    EXPECT_EQ(*it, 2);

    auto it2 = it + 4;
    EXPECT_EQ(*it2, 6);

    auto it3 = 3 + it;
    EXPECT_EQ(*it3, 5);

    auto it4 = it3 - 2;
    EXPECT_EQ(*it4, 3);
  }

  TEST(IntegralIteratorTest, RandomAccess) {
    IntegralIterator<int> it(10);
    EXPECT_EQ(it[0], 10);
    EXPECT_EQ(it[3], 13);
    EXPECT_EQ(it[5], 15);
  }

  TEST(IntegralIteratorTest, Difference) {
    IntegralIterator<int> a(10);
    IntegralIterator<int> b(4);
    EXPECT_EQ(a - b, 6);
    EXPECT_EQ(b - a, -6);
  }

  TEST(IntegralIteratorTest, Comparison) {
    IntegralIterator<int> a(5), b(7), c(5);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(a >= c);
    EXPECT_TRUE(b != a);
  }

  TEST(IntegralIteratorTest, Swap) {
    IntegralIterator<int> a(1), b(9);
    swap(a, b);
    EXPECT_EQ(*a, 9);
    EXPECT_EQ(*b, 1);
  }

  TEST(IntegralIteratorTest, BinarySearchExample) {
    IntegralIterator<int> first(0);
    IntegralIterator<int> last(100);

    // Find the first element > 80
    auto it = std::lower_bound(first, last, 81);

    EXPECT_EQ(*it, 81);
    EXPECT_EQ(it - first, 81);

    for (auto i = first; i != it; ++i) {
      EXPECT_LE(*i, 80);
    }

    for (auto i = it; i != last; ++i) {
      EXPECT_GE(*i, 81);
    }
  }
}
