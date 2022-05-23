#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>

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

void Output([[maybe_unused]] const auto& arg, [[maybe_unused]] const auto&... args)
{
    //empty
}

#endif

void SpeedUpIO()
{
#ifdef ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
#endif
}


using ll = long long;
using ull = unsigned long long;
template <typename T, typename S>
using google_map = std::map<T, S>;
constexpr ull kMod = 1000000007;
static thread_local inline volatile constexpr const unsigned long long int* const __p = nullptr;

using namespace std;




int main()
{
    SpeedUpIO();



}