#include "../DataStructure/flow_segment_tree.h"
#include <gtest/gtest.h>

#include <functional>
#include <limits>
#include <numeric>
#include <optional>
#include <utility>
#include <vector>

namespace flow {
  class SegmentTreeTest : public ::testing::Test {
  protected:
    static constexpr int kTimeoutMs = 5000; // 5s safety timeout

    int bruteSum(const std::vector<int>& v, std::size_t l, std::size_t r) {
      int s = 0;
      for (std::size_t i = l; i < r; ++i) s += v[i];
      return s;
    }

    int bruteMin(const std::vector<int>& v, std::size_t l, std::size_t r) {
      int m = std::numeric_limits<int>::max();
      for (std::size_t i = l; i < r; ++i) m = std::min(m, v[i]);
      return m;
    }

    int bruteMax(const std::vector<int>& v, std::size_t l, std::size_t r) {
      int M = std::numeric_limits<int>::min();
      for (std::size_t i = l; i < r; ++i) M = std::max(M, v[i]);
      return M;
    }
  };

  // -------------------- SUM tests --------------------

  TEST_F(SegmentTreeTest, Sum_Empty) {
    SegmentTree<int, std::plus<int>> st({}, std::plus<int>{});
    EXPECT_TRUE(st.empty());
    EXPECT_EQ(st.size(), 0u);
    auto r = st.getRange(0, 0);
    EXPECT_FALSE(r.has_value());
  }

  TEST_F(SegmentTreeTest, Sum_SingleElement) {
    SegmentTree<int, std::plus<int>> st({ 42 }, std::plus<int>{});
    EXPECT_FALSE(st.empty());
    EXPECT_EQ(st.size(), 1u);
    auto r = st.getRange(0, 1);
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(*r, 42);
  }

  TEST_F(SegmentTreeTest, Sum_AllAndPartial) {
    SegmentTree<int, std::plus<int>> st({ 1,2,3,4,5 }, std::plus<int>{});
    auto all = st.getRange(0, 5); ASSERT_TRUE(all); EXPECT_EQ(*all, 15);
    auto partial = st.getRange(1, 4); ASSERT_TRUE(partial); EXPECT_EQ(*partial, 9);
  }

  TEST_F(SegmentTreeTest, Sum_ZeroLengthRange) {
    SegmentTree<int, std::plus<int>> st({ 1,2,3 }, std::plus<int>{});
    auto r = st.getRange(2, 2);
    EXPECT_FALSE(r.has_value());
  }

  TEST_F(SegmentTreeTest, Sum_PointUpdateAndBoundaries) {
    std::vector<int> v{ 1,2,3,4,5 };
    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});
    // boundary updates
    st.setPoint(0, 10); v[0] = 10;
    st.setPoint(4, 20); v[4] = 20;
    auto all = st.getRange(0, v.size()); ASSERT_TRUE(all); EXPECT_EQ(*all, bruteSum(v, 0, v.size()));
    // middle update
    st.setPoint(2, 7); v[2] = 7;
    auto partial = st.getRange(1, 4); ASSERT_TRUE(partial); EXPECT_EQ(*partial, bruteSum(v, 1, 4));
  }

  TEST_F(SegmentTreeTest, Sum_NonPowerOfTwoSizes) {
    std::vector<int> v3{ 5,6,7 };
    SegmentTree<int, std::plus<int>> st3(v3.begin(), v3.end(), std::plus<int>{});
    auto r3 = st3.getRange(0, 3); ASSERT_TRUE(r3); EXPECT_EQ(*r3, bruteSum(v3, 0, 3));

    std::vector<int> v5{ 1,2,3,4,5 };
    SegmentTree<int, std::plus<int>> st5(v5.begin(), v5.end(), std::plus<int>{});
    auto r5 = st5.getRange(0, 5); ASSERT_TRUE(r5); EXPECT_EQ(*r5, bruteSum(v5, 0, 5));
  }

  TEST_F(SegmentTreeTest, Sum_RepeatedUpdatesAndQueries) {
    std::vector<int> v{ 0,0,0,0,0 };
    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});

    for (int i = 0; i < 5; ++i) {
      st.setPoint(i, i + 1); v[i] = i + 1;
      auto full = st.getRange(0, v.size()); ASSERT_TRUE(full);
      EXPECT_EQ(*full, bruteSum(v, 0, v.size()));
      auto half = st.getRange(1, 4); ASSERT_TRUE(half);
      EXPECT_EQ(*half, bruteSum(v, 1, 4));
    }
  }

  TEST_F(SegmentTreeTest, Sum_IteratorConstructorWorks) {
    std::vector<int> v{ 8,9,10 };
    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});
    auto r = st.getRange(0, 3); ASSERT_TRUE(r); EXPECT_EQ(*r, 27);
  }

  TEST_F(SegmentTreeTest, Sum_DeterministicStress) {
    const std::size_t N = 50;
    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 1); // 1..N
    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});

    for (int t = 0; t < 200; ++t) {
      std::size_t idx = (t * 7) % N;
      int newVal = static_cast<int>((t * 13) % 101);
      st.setPoint(idx, newVal);
      v[idx] = newVal;

      std::size_t l = (t * 11) % N;
      std::size_t r = l + ((t * 5) % (N - l));
      auto got = st.getRange(l, r);
      if (l == r) {
        EXPECT_FALSE(got.has_value());
      } else {
        ASSERT_TRUE(got.has_value());
        EXPECT_EQ(*got, bruteSum(v, l, r));
      }
    }
  }

  TEST_F(SegmentTreeTest, Sum_LargeInput) {
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 1);
    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});
    auto r = st.getRange(0, 1000); ASSERT_TRUE(r);
    EXPECT_EQ(*r, 1000 * 1001 / 2);
  }

  TEST_F(SegmentTreeTest, Sum_AllRanges_ThenUpdate) {
    constexpr std::size_t kN = 100;
    std::vector<int> v(kN);
    std::iota(v.begin(), v.end(), 1); // 1..100

    SegmentTree<int, std::plus<int>> st(v.begin(), v.end(), std::plus<int>{});

    // Check all ranges
    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) {
          EXPECT_FALSE(r.has_value());
        } else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteSum(v, i, j));
        }
      }
    }

    // Update each index and repeat
    for (std::size_t idx = 0; idx < kN; ++idx) {
      v[idx] = static_cast<int>((idx + 1) * 3);
      st.setPoint(idx, v[idx]);
    }

    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) {
          EXPECT_FALSE(r.has_value());
        } else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteSum(v, i, j));
        }
      }
    }
  }

  // -------------------- MIN tests --------------------

  TEST_F(SegmentTreeTest, Min_Empty) {
    auto minOp = [](int a, int b) { return std::min(a, b); };
    SegmentTree<int, decltype(minOp)> st({}, minOp);
    EXPECT_TRUE(st.empty());
    EXPECT_EQ(st.size(), 0u);
    auto r = st.getRange(0, 0);
    EXPECT_FALSE(r.has_value());
  }

  TEST_F(SegmentTreeTest, Min_BasicAndUpdates) {
    auto minOp = [](int a, int b) { return std::min(a, b); };
    SegmentTree<int, decltype(minOp)> st({ 5,3,8,6,7 }, minOp);
    auto all = st.getRange(0, 5); ASSERT_TRUE(all); EXPECT_EQ(*all, 3);
    auto partial = st.getRange(1, 4); ASSERT_TRUE(partial); EXPECT_EQ(*partial, 3);
    st.setPoint(1, 10); // 3 -> 10
    auto after = st.getRange(0, 5); ASSERT_TRUE(after); EXPECT_EQ(*after, 5);
    st.setPoint(0, 1); // 5 -> 1
    auto after2 = st.getRange(0, 5); ASSERT_TRUE(after2); EXPECT_EQ(*after2, 1);
  }

  TEST_F(SegmentTreeTest, Min_AllEqual) {
    auto minOp = [](int a, int b) { return std::min(a, b); };
    SegmentTree<int, decltype(minOp)> st({ 4,4,4,4 }, minOp);
    auto r = st.getRange(0, 4); ASSERT_TRUE(r); EXPECT_EQ(*r, 4);
  }

  TEST_F(SegmentTreeTest, Min_AllRanges_ThenUpdate) {
    constexpr std::size_t kN = 100;
    std::vector<int> v(kN);
    for (std::size_t i = 0; i < kN; ++i) v[i] = (i % 50) + 1; // repeat 1..50

    auto minOp = [](int a, int b) { return std::min(a, b); };
    SegmentTree<int, decltype(minOp)> st(v.begin(), v.end(), minOp);

    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) EXPECT_FALSE(r.has_value());
        else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteMin(v, i, j));
        }
      }
    }

    // Update each index and repeat
    for (std::size_t idx = 0; idx < kN; ++idx) {
      v[idx] = static_cast<int>(100 - idx); // descending values
      st.setPoint(idx, v[idx]);
    }

    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) EXPECT_FALSE(r.has_value());
        else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteMin(v, i, j));
        }
      }
    }
  }

  // -------------------- MAX tests --------------------

  TEST_F(SegmentTreeTest, Max_BasicAndUpdates) {
    auto maxOp = [](int a, int b) { return std::max(a, b); };
    SegmentTree<int, decltype(maxOp)> st({ 5,3,8,6,7 }, maxOp);
    auto all = st.getRange(0, 5); ASSERT_TRUE(all); EXPECT_EQ(*all, 8);
    auto partial = st.getRange(1, 4); ASSERT_TRUE(partial); EXPECT_EQ(*partial, 8);
    st.setPoint(2, 1); // 8 -> 1
    auto after = st.getRange(0, 5); ASSERT_TRUE(after); EXPECT_EQ(*after, 7);
    st.setPoint(3, 10); // 6 -> 10
    auto after2 = st.getRange(0, 5); ASSERT_TRUE(after2); EXPECT_EQ(*after2, 10);
  }

  TEST_F(SegmentTreeTest, Max_AllEqual) {
    auto maxOp = [](int a, int b) { return std::max(a, b); };
    SegmentTree<int, decltype(maxOp)> st({ 2,2,2 }, maxOp);
    auto r = st.getRange(0, 3); ASSERT_TRUE(r); EXPECT_EQ(*r, 2);
  }

  TEST_F(SegmentTreeTest, Max_AllRanges_ThenUpdate) {
    constexpr std::size_t kN = 100;
    std::vector<int> v(kN);
    for (std::size_t i = 0; i < kN; ++i) v[i] = static_cast<int>((i * 2) % 101);

    auto maxOp = [](int a, int b) { return std::max(a, b); };
    SegmentTree<int, decltype(maxOp)> st(v.begin(), v.end(), maxOp);

    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) EXPECT_FALSE(r.has_value());
        else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteMax(v, i, j));
        }
      }
    }

    // Update each index and repeat
    for (std::size_t idx = 0; idx < kN; ++idx) {
      v[idx] = static_cast<int>(idx % 30); // new values
      st.setPoint(idx, v[idx]);
    }

    for (std::size_t i = 0; i < kN; ++i) {
      for (std::size_t j = i; j <= kN; ++j) {
        auto r = st.getRange(i, j);
        if (i == j) EXPECT_FALSE(r.has_value());
        else {
          ASSERT_TRUE(r.has_value());
          EXPECT_EQ(*r, bruteMax(v, i, j));
        }
      }
    }
  }
}
