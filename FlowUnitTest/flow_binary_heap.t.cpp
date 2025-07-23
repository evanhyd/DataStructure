#include "../DataStructure/flow_vector.h" // DO NOT REMOVE OR REORDER THIS, COMPILER BUG
#include "../DataStructure/flow_binary_heap.h"
#include <functional>
#include <gtest/gtest.h>
#include <string>

namespace flow {

  class BinaryHeapTest : public ::testing::Test {};

  // Test int with default std::less<>
  TEST_F(BinaryHeapTest, IntDefaultComparator) {
    BinaryHeap<int> heap;
    EXPECT_TRUE(heap.empty());
    heap.push(10);
    heap.push(5);
    heap.push(7);
    EXPECT_EQ(heap.top(), 5);
    EXPECT_EQ(heap.size(), 3u);

    int popped = heap.pop();
    EXPECT_EQ(popped, 5);
    EXPECT_EQ(heap.top(), 7);
    heap.drop();
    EXPECT_EQ(heap.size(), 1u);
    EXPECT_EQ(heap.top(), 10);

    heap.clear();
    EXPECT_TRUE(heap.empty());
  }

  // Test std::string with default comparator (lex order)
  TEST_F(BinaryHeapTest, StringDefaultComparator) {
    BinaryHeap<std::string> heap;
    heap.push("pear");
    heap.push("apple");
    heap.push("orange");

    EXPECT_EQ(heap.top(), "apple");
    EXPECT_EQ(heap.size(), 3u);

    auto popped = heap.pop();
    EXPECT_EQ(popped, "apple");
    EXPECT_EQ(heap.top(), "orange");
  }

  // Non-default-constructible comparator (stateful)
  struct NonDefaultComp {
    int bias;
    explicit NonDefaultComp(int b) : bias(b) {}
    bool operator()(int a, int b) const {
      // Reverse order + bias to test comparator works
      return (a + bias) > (b + bias);
    }
  };

  TEST_F(BinaryHeapTest, NonDefaultConstructibleComparator) {
    NonDefaultComp comp(1);
    BinaryHeap<int, NonDefaultComp> heap(comp);
    heap.push(1);
    heap.push(3);
    heap.push(2);

    // With bias=1, order reversed with offset, so smallest becomes largest
    EXPECT_EQ(heap.top(), 3);
    heap.pop();
    EXPECT_EQ(heap.top(), 2);
  }

  // Lambda comparator with capture
  TEST_F(BinaryHeapTest, LambdaComparatorWithCapture) {
    int offset = 10;
    auto comp = [offset](int a, int b) {
      return (a + offset) > (b + offset); // Reverse order with offset
    };
    BinaryHeap<int, decltype(comp)> heap(comp);

    heap.push(1);
    heap.push(5);
    heap.push(3);

    EXPECT_EQ(heap.top(), 5);
    heap.pop();
    EXPECT_EQ(heap.top(), 3);
  }

} // namespace flow
