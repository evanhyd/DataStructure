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
#include <type_traits>

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
  return __builtin_popcount(n);
  #endif
}

#include "memory.h"
#include "random.h"
#include "time.h"
#include "SkipList.h"

using namespace std;
using namespace box;


int main() {
  MemoryGuard();
  SkipList<int, string> list;
  list.Insert({ 10, "a" });
  list.Insert({ 5, "b" });
  list.Insert({ 7, "c" });
  list.Print();
  list.Erase(101);
  list.Print();


  return 0;
}

#endif

/*
priority queue is a max heap
sort the data to find the pattern
vec.size() - 1 may underflow
*/
