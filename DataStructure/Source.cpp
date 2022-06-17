#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cctype>

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <bit>

#include <regex>
#include <any>
#include <cmath>
#include <ctime>
#include <climits>
#include <cassert>
#include <numeric>
#include <chrono>
#include <random>
#include <memory>
#include <thread>
#include <algorithm>
#include <functional>
#include <execution>
#include <source_location>



#ifndef ONLINE_JUDGE

#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#endif

#include "DynamicArray.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "PSDA.h"
#include "SegmentTree.h"
#include "SparseTable.h"
#include "Tuple.h"
#include "DisjointSet.h"

#include "io.h"
#include "memory.h"
#include "time.h"
#include "random.h"
#include "graph.h"

using namespace cug::io;
using namespace cug::memory;
using namespace cug::time;
using namespace cug::random;
using namespace cug::graph;

#else

//not portable
#if defined _WIN32 || defined _WIN64
inline char getchar_unlocked() { return static_cast<char>(_getchar_nolock()); }
inline void putchar_unlocked(char c) { _putchar_nolock(c); }
#endif
template <std::signed_integral T>
T Read()
{
    T x; bool neg = false; char c{};
    do { c = getchar_unlocked(); if (c == '-') neg = true; } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0') {}
    return neg ? -x : x;
}

template <std::unsigned_integral T>
T Read()
{
    T x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

void Output([[maybe_unused]] const auto& arg, [[maybe_unused]] const auto&... args) {}

#endif





template <typename T, typename S>
using google_map = std::map<T, S>;
static thread_local inline volatile constexpr const unsigned long long int* const _A_A_ = nullptr;


using ll = long long;
using ull = unsigned long long;

constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293 };

constexpr auto Pow(std::integral auto base, std::integral auto power)
{
    using T = decltype(base);
    T product = 1;
    constexpr T kPowExp = 2;
    while (power > 0)
    {
        if (power % kPowExp) product *= base;
        base *= base;
        power /= kPowExp;
    }
    return product;
}

using namespace std;


//int main()
//{
//#ifdef ONLINE_JUDGE
//    std::cin.tie(nullptr)->sync_with_stdio(false);
//#endif
//
//
//}

/* 
overflow?
array bounds?
edge case n = 0, n = 1?
DS reset properly between test cases?
*/