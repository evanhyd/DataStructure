#include "../DataStructure/flow_random_algorithm.h"
#include <gtest/gtest.h>

#include <numeric>
#include <vector>

namespace flow {

  TEST(RandomAlgorithmTest, GetRandomIntWithinBounds) {
    for (int i = 0; i < 100; ++i) {
      int value = getRandomNumber(10, 20);
      EXPECT_GE(value, 10);
      EXPECT_LE(value, 20);
    }
  }

  TEST(RandomAlgorithmTest, GetRandomFloatWithinBounds) {
    for (int i = 0; i < 100; ++i) {
      float value = getRandomNumber(1.5f, 3.5f);
      EXPECT_GE(value, 1.5f);
      EXPECT_LE(value, 3.5f);
    }
  }

  TEST(RandomAlgorithmTest, GetRandomDoubleWithinBounds) {
    for (int i = 0; i < 100; ++i) {
      double value = getRandomNumber(-5.0, 5.0);
      EXPECT_GE(value, -5.0);
      EXPECT_LE(value, 5.0);
    }
  }

  TEST(RandomAlgorithmTest, GetRandomIntDegenerateRange) {
    int value = getRandomNumber(42, 42);
    EXPECT_EQ(value, 42);
  }

  TEST(RandomAlgorithmTest, GetRandomFloatDegenerateRange) {
    float value = getRandomNumber(3.14f, 3.14f);
    EXPECT_FLOAT_EQ(value, 3.14f);
  }

  TEST(RandomAlgorithmTest, ShuffleVectorPreservesElements) {
    std::vector<int> original(100);
    std::iota(original.begin(), original.end(), 0);

    std::vector<int> shuffled = original;
    shuffle(shuffled.begin(), shuffled.end());

    sort(shuffled.begin(), shuffled.end());
    EXPECT_EQ(original, shuffled);
  }

  TEST(RandomAlgorithmTest, ShuffleActuallyChangesOrder) {
    std::vector<int> vec(100);
    std::iota(vec.begin(), vec.end(), 0);

    std::vector<int> shuffled = vec;
    shuffle(shuffled.begin(), shuffled.end());

    size_t identicalCount = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
      if (vec[i] == shuffled[i]) {
        ++identicalCount;
      }
    }

    // It is technically possible for identicalCount == vec.size(), but extremely unlikely
    EXPECT_LT(identicalCount, vec.size());
  }
}
