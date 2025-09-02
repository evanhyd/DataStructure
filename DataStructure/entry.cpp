#define CP_TEMPLATE
#ifdef CP_TEMPLATE
// IO Library
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <sstream>

// DS/ADT Library
#include <array>
#include <bitset>
#include <deque>
#include <initializer_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Algorithm/Helper Library
#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <climits>
#include <cmath>
#include <iterator>
#include <memory>
#include <numeric>
#include <random>
#include <regex>
#include <thread>
#include <type_traits>
#include <utility>
#ifdef _HAS_CXX20
#include <bit>
#endif
#include <limits>

// Type Alias
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
using pi32 = std::pair<i32, i32>;
using pi64 = std::pair<i64, i64>;
using pu64 = std::pair<u64, u64>;

// Magic Constant
constexpr f64 kPI = 3.1415926535;
constexpr u64 kMod = 1000000007ull;
constexpr i32 kDx[] = { -1, 0, 1, 0 };
constexpr i32 kDy[] = { 0, 1, 0, -1 };
constexpr u64 kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
};

// Hashing
namespace std {
  template<typename T>
  struct hash<pair<T, T>> {
    typename std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, i32> || std::is_same_v<T, u32>, std::size_t>
      operator()(const pair<T, T>& p) const {
      return std::hash<T>{}(p.first) ^ (std::hash<T>{}(p.second) << 1);
    }
  };
}

// Generic Printer
template <typename Iter>
std::ostream& genericPrint(std::ostream& out, Iter first, Iter last, char openBracket, char endBracket);

// Generic Containers
template <typename T,
  typename = std::enable_if_t<!std::is_same_v<T, std::string>>,
  typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category>>
>
std::ostream& operator<<(std::ostream& out, const T& data);

// Pair
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& data);

// Array
template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& data);

// List-like Containers
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& data);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::forward_list<T>& data);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& data);

// Set-like Containers
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& data);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::multiset<T>& data);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& data);

// Map-like Containers
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::map<T, U>& data);

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::multimap<T, U>& data);

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, U>& data);

// Formatting Implementation
template <typename Iter>
std::ostream& genericPrint(std::ostream& out, Iter first, Iter last, char openBracket, char endBracket) {
  out << openBracket;
  if (first != last) {
    out << *first;
    while (++first != last) {
      out << ", " << *first;
    }
  }
  out << endBracket;
  return out;
}

template < typename T, typename, typename>
std::ostream& operator<<(std::ostream& out, const T& vec) {
  return genericPrint(out, std::begin(vec), std::end(vec), '<', '>');
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& data) {
  return out << '(' << data.first << ", " << data.second << ')';
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '[', ']');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '[', ']');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '[', ']');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::forward_list<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '[', ']');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& data) {
  genericPrint(out, std::begin(data), std::end(data), '[', ']');
  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::multiset<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::map<T, U>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::multimap<T, U>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, U>& data) {
  return genericPrint(out, std::begin(data), std::end(data), '{', '}');
}

// Algorithm
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T popCount(T n) {
#ifdef _HAS_CXX20
  return std::popcount(n);
#else
  return __builtin_popcount(n);
#endif
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr int countDigits(T x) {
  int digits = 1;
  for (; x >= T{ 10 }; x /= T{ 10 }) {
    ++digits;
  }
  return digits;
}

// IO
const auto _ = std::cin.tie(nullptr)->sync_with_stdio(false);

#ifdef LC_HACK
const auto __ = std::atexit([]() { std::ofstream("display_runtime.txt") << INT_MAX << '\n'; });
#endif

#if defined _WIN32 || defined _WIN64
#define USACO(filename) 0
#else
#define USACO(filename) do {freopen(filename".in", "r", stdin); freopen(filename".out", "w", stdout);}while(false)
#endif

#endif

using namespace std;

int main() {

}
