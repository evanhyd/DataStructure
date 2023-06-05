#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <climits>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define IN_USE
#ifdef IN_USE

using ll = long long;
using ull = unsigned long long;

constexpr float kPi = 3.1415926535f;
constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
};

template <typename T>
constexpr T popcount(T n) {
#ifdef _HAS_CXX20
  return std::popcount(n);
#else
  return __builtin__popcount(n);
#endif
}

using namespace std;


#include "LinkedList.h"
#include "memory.h"
#include "BinaryHeap.h"
#include "random.h"

struct MinMaxHeap {
  vector<int> h1_;
  vector<int> h2_;
  vector<int> t1_;
  vector<int> t2_;


  int Parent(int n) {
    return (n - 1) / 2;
  }

  int LeftChild(int n) {
    return n * 2 + 1;
  }

  int RightChild(int n) {
    return n * 2 + 2;
  }

public:
  void Push(int n) {
    t1_.push_back(h2_.size() - 1);
    t2_.push_back(h1_.size() - 1);

    h1_.push_back(n);
    for (int i = h1_.size() - 1; i > 0;) {
      int parent = Parent(i);
      if (h1_[i] < h1_[parent]) {
        swap(h1_[i], h1_[parent]);
        swap(t1_[i], t1_[parent]);
        swap(t2_[t1_[i]], t2_[t1_[parent]]);
      } else {
        break;
      }
    }

    h2_.push_back(n);
    for (int i = h2_.size() - 1; i > 0;) {
      int parent = Parent(i);
      if (h2_[i] > h2_[parent]) {
        swap(h2_[i], h2_[parent]);
        swap(t2_[i], t2_[parent]);
        swap(t1_[t2_[i]], t1_[t2_[parent]]);
      } else {
        break;
      }
    }
  }

  int Min() {
    return h1_[0];
  }

  int Max() {
    return h2_[0];
  }
};

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  box::MemoryGuard();

  MinMaxHeap heap;
  BinaryHeap<int> maxHeap;
  BinaryHeap<int, std::less<int>>  minHeap;

  for (int i = 0; i < 1000; ++i) {
    int n = box::GetRandom(0, 100000);
    heap.Push(n);
    maxHeap.Push(n);
    minHeap.Push(n);

    assert(heap.Min() == minHeap.Top());
    assert(heap.Max() == maxHeap.Top());
  }
}

#endif

/*
priority queue is a max heap
sort the data to find the pattern
*/