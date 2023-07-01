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

#include "memory.h"
#include "random.h"
#include "time.h"

#include "BinomialHeap.h"

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  box::MemoryGuard();

  BinomialHeap<int> h;
  box::Timer timer;
  timer.Start();
  for (int i = 0; i < 1000000; ++i) {
    h.Push(i % 1024);
  }
  timer.Stop();
  timer.Print();

  timer.Start();
  while (!h.Empty()) {
    h.Pop();
  }
  timer.Stop();
  timer.Print();
}

#endif

/*
priority queue is a max heap
sort the data to find the pattern
*/