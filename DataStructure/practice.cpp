#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cassert>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <numeric>
#include <bitset>
#include <chrono>
#include <random>
#include <functional>
#include <regex>
#include <climits>
#include <type_traits>

#include "time.h"
#include "random.h"
#include "memory.h"

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

int main(){ 
  box::MemoryGuard();

  box::Timer timer;
  timer.Start();
  vector<int*> vec(1000);
  for (auto n : vec) {
    n = box::Pool::Allocate<int>(1, 10);
  }
  timer.Stop();
  timer.Print();
  box::Pool::PrintPool();
}




#endif

/*
priority queue is a max heap
sort the data to find the pattern
*/