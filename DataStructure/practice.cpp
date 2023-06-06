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
#include "RandomMeldableHeap.h"

struct MinMaxHeap {
  vector<int> H_1;
  vector<int> H_2;
  vector<int> T_1;
  vector<int> T_2;


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
    T_1.push_back(H_2.size());
    T_2.push_back(H_1.size());

    H_1.push_back(n);
    for (int i = H_1.size() - 1; i > 0;) {
      int parent = Parent(i);
      if (H_1[i] < H_1[parent]) {
        swap(H_1[i], H_1[parent]);
        swap(T_1[i], T_1[parent]);
        swap(T_2[T_1[i]], T_2[T_1[parent]]);
        i = parent;
      } else {
        break;
      }
    }

    H_2.push_back(n);
    for (int i = H_2.size() - 1; i > 0;) {
      int parent = Parent(i);
      if (H_2[i] > H_2[parent]) {
        swap(H_2[i], H_2[parent]);
        swap(T_2[i], T_2[parent]);
        swap(T_1[T_2[i]], T_1[T_2[parent]]);
        i = parent;
      } else {
        break;
      }
    }

    assert(H_1.size() == H_2.size());
    for (int i = 0; i < H_1.size(); ++i) {
      assert(H_1[i] == H_2[T_1[i]]);
      assert(H_2[i] == H_1[T_2[i]]);
    }
  }

  void Delete(int i) {
    while (i > 0) {
      int parent = (i - 1) / 2;
      swap(H_1[i], H_1[parent]);
      swap(T_1[i], T_1[parent]);
      swap(T_2[T_1[i]], T_2[T_1[parent]]);
      i = parent;
    }

    swap(H_1[0], H_1.back());
    swap(T_1[0], T_1.back());
    swap(T_2[T_1[0]], T_2[T_1.back()]);
    H_1.pop_back();

    while (true) {
      int left = i * 2 + 1;
      if (left >= H_1.size()) {
        break;
      }

      int right = left + 1;
      if (right < H_1.size() && H_1[right] < H_1[left]) {
        left = right;
      }

      if (H_1[left] < H_1[i]) {
        swap(H_1[i], H_1[left]);
        swap(T_1[i], T_1[left]);
        swap(T_2[T_1[i]], T_2[T_1[left]]);
        i = left;
      } else {
        break;
      }
    }


    //j is the index to be removed in the max heap
    int j = T_1.back();
    while (j > 0) {
      int parent = (j - 1) / 2;
      swap(H_2[j], H_2[parent]);
      swap(T_2[j], T_2[parent]);
      swap(T_1[T_2[j]], T_1[T_2[parent]]);
      j = parent;
    }

    swap(H_2[0], H_2.back());
    swap(T_2[0], T_2.back());
    swap(T_1[T_2[0]], T_1[T_2.back()]);
    H_2.pop_back();

    while (true) {
      int left = j * 2 + 1;
      if (left >= H_2.size()) {
        break;
      }

      int right = left + 1;
      if (right < H_2.size() && H_2[left] < H_2[right]) {
        left = right;
      }

      if (H_2[left] > H_2[j]) {
        swap(H_2[j], H_2[left]);
        swap(T_2[j], T_2[left]);
        swap(T_1[T_2[j]], T_1[T_2[left]]);
        j = left;
      } else {
        break;
      }
    }

    T_1.pop_back();
    T_2.pop_back();

    assert(H_1.size() == H_2.size());
    for (int i = 0; i < H_1.size(); ++i) {
      assert(H_1[i] == H_2[T_1[i]]);
      assert(H_2[i] == H_1[T_2[i]]);
    }
  }

  void DeleteMin() {
    Delete(0);
  }

  void DeleteMax() {
    Delete(T_2[0]);
  }

  int Min() {
    return H_1[0];
  }

  int Max() {
    return H_2[0];
  }
};



//int main() {
//  cin.tie(nullptr)->sync_with_stdio(false);
//  box::MemoryGuard();
//
//  MinMaxHeap heap;
//  BinaryHeap<int> maxHeap;
//  BinaryHeap<int, std::less<int>>  minHeap;
//
//  for (int i = 0; i < 50; ++i) {
//    int n = box::GetRandom(0, 20);
//    heap.Push(n);
//    maxHeap.Push(n);
//    minHeap.Push(n);
//
//    assert(heap.Max() == maxHeap.Top());
//    assert(heap.Min() == minHeap.Top());
//  }
//
//  for (int i = 0; i < 50; ++i) {
//    assert(heap.Max() == maxHeap.Top());
//    cout << heap.Max() << ' ' << maxHeap.Top() << '\n';
//    heap.DeleteMax();
//    maxHeap.Pop();
//
//    /*assert(heap.Min() == minHeap.Top());
//    cout << heap.Min() << ' ' << minHeap.Top() << '\n';
//    heap.DeleteMin();
//    minHeap.Pop();*/
//  }
//}

int main() {
  box::MemoryGuard();

  RandomMeldableHeap<int> h;
  for (int i = 0; i < 100000; ++i) {
    int n = box::GetRandom(0, 1000000);
    h.Push(n);
  }

  int prev = INT_MAX;
  while (h.Size()) {
    assert(prev >= h.Top());
    prev = h.Top();
    h.Pop();
  }
}

#endif

/*
priority queue is a max heap
sort the data to find the pattern
*/