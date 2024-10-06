#define IN_USE
#ifdef IN_USE

#define CP_TEMPLATE
#ifdef CP_TEMPLATE
//io
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>

//DS && ADT
#include <string>
#include <array>
#include <vector>
#include <deque>
#include <bitset>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <tuple>

//algorithm && helper
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>
#include <chrono>
#include <iterator>
#include <random>
#include <memory>
#include <numeric>
#include <thread>
#include <type_traits>
#include <utility>
#ifdef _HAS_CXX20
#include <bit>
#endif

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;
using pii = std::pair<i32, i32>;
using pll = std::pair<i64, i64>;
using puu = std::pair<u64, u64>;

constexpr f64 kPI = 3.1415926535;
constexpr u64 kMod = 1000000007ull;
constexpr i32 kDx[] = { 0, 0, -1, 1 };
constexpr i32 kDy[] = { -1, 1, 0, 0 };
constexpr u64 kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
};

namespace std {
  template<typename T>
  struct hash<pair<T, T>> {
    typename std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, i32> || std::is_same_v<T, u32>, std::size_t>
      operator()(const pair<T, T>& p) const {
      return std::hash<T>{}(p.first) ^ (std::hash<T>{}(p.second) << 1);
    }
  };
}

#if defined _WIN32 || defined _WIN64
template <
  typename T,
  std::enable_if_t<!std::is_same_v<T, std::string>, int> = 0,
  std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category>, int> = 0
>
std::ostream& operator<<(std::ostream& out, const T& vec) {
  auto it = vec.begin();
  if (it == vec.end()) {
    return out;
  }
  out << *it;
  while (++it != vec.end()) {
    out << ' ' << *it;
  }
  return out;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& vec) {
  out << '(' << vec.first << ' ' << vec.second << ')';
  return out;
}
#endif

template <typename T, std::enable_if_t<std::is_unsigned_v<T>, int> = 0>
constexpr T PopCount(T n) {
#ifdef _HAS_CXX20
  return std::popcount(n);
#else
  return __builtin_popcount(n);
#endif
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
constexpr int CountDigits(T x) {
  int digits = 1;
  for (; x >= T{ 10 }; x /= T{ 10 }) {
    ++digits;
  }
  return digits;
}

const auto _ = std::cin.tie(nullptr)->sync_with_stdio(false);

#if defined _WIN32 || defined _WIN64
#define USACO(filename) 0
#else
#define USACO(filename) freopen(filename".in", "r", stdin); freopen(filename".out", "w", stdout)
#endif

#endif

using namespace std;

#include "memory.h"
#include "Vector.h"
#include "Allocator.h"

int main() {
  box::MemoryGuard();

  flow::Vector<int> a = { 1, 2, 3, 4 };
  auto b = a.map([](int n) {
    string s = string(n, 'a');
    return s;
    });

  for (int i = 0; i < b.size(); ++i) {
    cout << b[i] << '\n';
  }

}

/*
priority queue is a max heap
sort the data to find the pattern
vec.size() - 1 may underflow
*/

#endif
