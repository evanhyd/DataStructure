#include <gtest/gtest.h>
#include "../DataStructure/flow_disjoint_set.h"

namespace flow {

  // Helper function to fill a DisjointSet with n elements
  template <typename T>
  DisjointSet<T> makeSet(std::size_t n) {
    DisjointSet<T> ds;
    for (std::size_t i = 0; i < n; ++i) {
      ds.add(static_cast<T>(i));
    }
    return ds;
  }

  // =====================================================
  // Test Cases
  // =====================================================

  TEST(DisjointSetTest, EmptySet) {
    DisjointSet<int> ds;
    EXPECT_TRUE(ds.empty());
    EXPECT_EQ(ds.size(), 0);
    EXPECT_EQ(ds.groupSize(), 0);
  }

  TEST(DisjointSetTest, SingleElement) {
    DisjointSet<int> ds;
    ds.add(42);
    EXPECT_FALSE(ds.empty());
    EXPECT_EQ(ds.size(), 1);
    EXPECT_EQ(ds.groupSize(), 1);
    EXPECT_EQ(ds[0], 42);
    EXPECT_TRUE(ds.isSameGroup(0, 0));
  }

  TEST(DisjointSetTest, MultipleElementsInitialGroups) {
    auto ds = makeSet<int>(5);
    EXPECT_EQ(ds.size(), 5);
    EXPECT_EQ(ds.groupSize(), 5);

    for (std::size_t i = 0; i < 5; ++i) {
      EXPECT_TRUE(ds.isSameGroup(i, i));
      for (std::size_t j = 0; j < 5; ++j) {
        if (i != j) {
          EXPECT_FALSE(ds.isSameGroup(i, j));
        }
      }
    }
  }

  TEST(DisjointSetTest, MergeTwoElements) {
    auto ds = makeSet<int>(3);
    EXPECT_TRUE(ds.merge(0, 1));
    EXPECT_TRUE(ds.isSameGroup(0, 1));
    EXPECT_FALSE(ds.isSameGroup(0, 2));
    EXPECT_EQ(ds.groupSize(), 2);

    // Merging again returns false
    EXPECT_FALSE(ds.merge(0, 1));
    EXPECT_EQ(ds.groupSize(), 2);
  }

  TEST(DisjointSetTest, MergeMultipleElements) {
    auto ds = makeSet<int>(4);
    EXPECT_TRUE(ds.merge(0, 1));
    EXPECT_TRUE(ds.merge(1, 2));
    EXPECT_TRUE(ds.merge(2, 3));

    for (std::size_t i = 0; i < 4; ++i) {
      for (std::size_t j = 0; j < 4; ++j) {
        EXPECT_TRUE(ds.isSameGroup(i, j));
      }
    }

    EXPECT_EQ(ds.groupSize(), 1);
  }

  TEST(DisjointSetTest, AddAfterMerges) {
    auto ds = makeSet<int>(2);
    ds.merge(0, 1);
    ds.add(99);

    EXPECT_EQ(ds.size(), 3);
    EXPECT_EQ(ds.groupSize(), 2);

    EXPECT_TRUE(ds.isSameGroup(0, 1));
    EXPECT_FALSE(ds.isSameGroup(0, 2));
    EXPECT_EQ(ds[2], 99);
  }

  TEST(DisjointSetTest, SelfMergeDoesNothing) {
    auto ds = makeSet<int>(1);
    EXPECT_FALSE(ds.merge(0, 0));
    EXPECT_EQ(ds.groupSize(), 1);
  }

  TEST(DisjointSetTest, FindPathCompressionEffect) {
    auto ds = makeSet<int>(5);
    ds.merge(0, 1);
    ds.merge(1, 2);
    ds.merge(2, 3);

    std::size_t root_before = ds.find(3);
    std::size_t root_after = ds.find(3);

    EXPECT_EQ(root_before, root_after);

    // After path compression, all nodes point directly to root
    std::size_t root = ds.find(0);
    for (std::size_t i = 0; i < 4; ++i) {
      EXPECT_EQ(ds.find(i), root);
    }
  }

  TEST(DisjointSetTest, MultipleMergesDifferentRanks) {
    auto ds = makeSet<int>(6);
    ds.merge(0, 1); // rank of new root = 1
    ds.merge(2, 3); // rank of new root = 1
    ds.merge(0, 2); // should merge smaller rank tree into larger rank

    EXPECT_EQ(ds.groupSize(), 3);
    EXPECT_TRUE(ds.isSameGroup(0, 3));
    EXPECT_FALSE(ds.isSameGroup(0, 4));
  }

  TEST(DisjointSetTest, InitializerListConstructor) {
    DisjointSet<int> ds{ 10, 20, 30 };
    EXPECT_EQ(ds.size(), 3);
    EXPECT_EQ(ds.groupSize(), 3);
    EXPECT_EQ(ds[0], 10);
    EXPECT_EQ(ds[1], 20);
    EXPECT_EQ(ds[2], 30);
  }

  // =====================================================
// Stress / Large-scale test
// =====================================================
  TEST(DisjointSetTest, LargeSetAllMerges) {
    constexpr std::size_t N = 100;
    auto ds = makeSet<int>(N);

    // Merge every pair in sequence
    for (std::size_t i = 0; i < N - 1; ++i) {
      EXPECT_TRUE(ds.merge(i, i + 1));
    }

    // After merging everything, all should be in the same group
    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < N; ++j) {
        EXPECT_TRUE(ds.isSameGroup(i, j));
      }
    }

    EXPECT_EQ(ds.groupSize(), 1);
  }

  TEST(DisjointSetTest, AddAfterLargeMerges) {
    constexpr std::size_t N = 50;
    auto ds = makeSet<int>(N);

    // Merge half into one group
    for (std::size_t i = 0; i < N / 2 - 1; ++i) {
      ds.merge(i, i + 1);
    }

    ds.add(999);

    EXPECT_EQ(ds.size(), N + 1);
    EXPECT_EQ(ds.groupSize(), N / 2 + 2);

    EXPECT_TRUE(ds.isSameGroup(0, N / 2 - 1));
    EXPECT_FALSE(ds.isSameGroup(0, N)); // last element
    EXPECT_EQ(ds[N], 999);
  }

  TEST(DisjointSetTest, RandomMergeSequence) {
    auto ds = makeSet<int>(10);
    
    EXPECT_TRUE(ds.merge(0, 3));
    EXPECT_TRUE(ds.merge(4, 5));
    EXPECT_TRUE(ds.merge(0, 4));
    EXPECT_TRUE(ds.merge(6, 7));
    EXPECT_TRUE(ds.merge(6, 1));
    EXPECT_TRUE(ds.merge(0, 6));

    EXPECT_TRUE(ds.isSameGroup(0, 1));
    EXPECT_TRUE(ds.isSameGroup(3, 7));
    EXPECT_FALSE(ds.isSameGroup(0, 8));
    EXPECT_EQ(ds.groupSize(), 4);
  }

  TEST(DisjointSetTest, SelfMergesAndDuplicates) {
    auto ds = makeSet<int>(5);

    // Self merge should return false
    EXPECT_FALSE(ds.merge(0, 0));
    EXPECT_EQ(ds.groupSize(), 5);

    // Duplicate merges
    EXPECT_TRUE(ds.merge(0, 1));
    EXPECT_FALSE(ds.merge(1, 0));
    EXPECT_EQ(ds.groupSize(), 4);

    EXPECT_TRUE(ds.merge(2, 3));
    EXPECT_FALSE(ds.merge(3, 2));
    EXPECT_EQ(ds.groupSize(), 3);
  }

  TEST(DisjointSetTest, AllPairsMergeCheck) {
    constexpr std::size_t N = 10;
    auto ds = makeSet<int>(N);

    // Merge all into one group
    for (std::size_t i = 1; i < N; ++i) {
      ds.merge(0, i);
    }

    // Verify all pairs
    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < N; ++j) {
        EXPECT_TRUE(ds.isSameGroup(i, j));
      }
    }

    EXPECT_EQ(ds.groupSize(), 1);
  }

  TEST(DisjointSetTest, PathCompressionCheck) {
    auto ds = makeSet<int>(6);

    // Chain merges
    ds.merge(0, 1);
    ds.merge(1, 2);
    ds.merge(2, 3);
    ds.merge(3, 4);
    ds.merge(4, 5);

    // Before path compression: manually traverse
    std::size_t root_before = ds.find(5);
    EXPECT_EQ(root_before, ds.find(0));

    // Multiple repeated finds should not change root
    for (int i = 0; i < 10; ++i) {
      EXPECT_EQ(ds.find(5), root_before);
      EXPECT_EQ(ds.find(3), root_before);
      EXPECT_EQ(ds.find(1), root_before);
    }
  }

  TEST(DisjointSetTest, IterativeConsistency) {
    constexpr std::size_t N = 20;
    auto ds = makeSet<int>(N);

    for (std::size_t i = 0; i < N - 1; i += 2) {
      ds.merge(i, i + 1);
    }

    // Check each pair
    for (std::size_t i = 0; i < N; i += 2) {
      EXPECT_TRUE(ds.isSameGroup(i, i + 1));
      if (i + 2 < N) {
        EXPECT_FALSE(ds.isSameGroup(i, i + 2));
      }
    }
  }

  TEST(DisjointSetTest, MixedAddAndMerge) {
    auto ds = makeSet<int>(3);
    ds.add(100); // index 3
    ds.add(200); // index 4

    EXPECT_TRUE(ds.merge(0, 3));
    EXPECT_TRUE(ds.merge(1, 4));

    EXPECT_TRUE(ds.isSameGroup(0, 3));
    EXPECT_TRUE(ds.isSameGroup(1, 4));
    EXPECT_FALSE(ds.isSameGroup(0, 1));
    EXPECT_EQ(ds.groupSize(), 3);
  }

} // namespace flow
