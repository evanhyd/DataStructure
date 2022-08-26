#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>


#include "Trie.h"

#ifdef ONLINE_JUDGE

// not portable
#if defined _WIN32 || defined _WIN64
inline char getchar_unlocked() { return static_cast<char>(_getchar_nolock()); }
inline void putchar_unlocked(char c) { _putchar_nolock(c); }
#endif
template <std::signed_integral T>
T Read() {
  T x;
  bool neg = false;
  char c{};
  do {
    c = getchar_unlocked();
    if (c == '-') neg = true;
  } while (c < '0');
  for (x = c - '0'; '0' <= (c = getchar_unlocked());
       x = (x << 3) + (x << 1) + c - '0') {
  }
  return neg ? -x : x;
}

template <std::unsigned_integral T>
T Read() {
  T x;
  char c{};
  do {
    c = getchar_unlocked();
  } while (c < '0');
  for (x = c - '0'; '0' <= (c = getchar_unlocked());
       x = (x << 3) + (x << 1) + c - '0')
    ;
  return x;
}

void Output([[maybe_unused]] const auto& arg,
            [[maybe_unused]] const auto&... args) {}

#endif

using ll = long long;
using ull = unsigned long long;

constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101,
    103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
    241, 251, 257, 263, 269, 271, 277, 281, 283, 293};

using namespace std;





int main() {
#ifdef ONLINE_JUDGE
  std::cin.tie(nullptr)->sync_with_stdio(false);
#endif


  string hello = "hello";
  Trie<char> t;
  cout << t.Contain(hello.c_str(), hello.size()) << '\n';

  t.Insert(hello.c_str(), hello.size());
  t.Insert("hel", 3);
  cout << t.Contain(hello.c_str(), hello.size()) << '\n';
  cout << t.Contain("hel", 3) << '\n';
  cout << t.Contain("hea", 3) << '\n';
  cout << t.ContainPrefix(hello.c_str(), hello.size()) << '\n';
  cout << t.ContainPrefix("hel", 3) << '\n';
  cout << t.ContainPrefix("hea", 3) << '\n';

  t.Erase(hello.c_str(), hello.size());
  cout << t.Contain(hello.c_str(), hello.size()) << '\n';
  cout << t.Contain("hel", 3) << '\n';
  cout << t.Contain("hea", 3) << '\n';
  cout << t.ContainPrefix(hello.c_str(), hello.size()) << '\n';
  cout << t.ContainPrefix("hel", 3) << '\n';
  cout << t.ContainPrefix("hea", 3) << '\n';
}

/*
overflow
array bounds
edge case n = 0, n = 1
reset between test cases
*/