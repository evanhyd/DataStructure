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





using ll = int64_t;
using ull = uint64_t;

template <typename T, typename S>
using google_map = std::map<T, S>;
static thread_local inline volatile constexpr const unsigned long long int* const __p = nullptr;


constexpr ull kMod = 1000000007;
constexpr ull kPrimes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293 };

constexpr auto Pow(auto base, auto power)
{
    auto product = 1;
    while (power > 0)
    {
        if (power % 2) product *= base;
        base *= base;
        power /= 2;
    }

    return product;
}

using namespace std;


class Solution
{
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges)
    {
        DisjointSet<int> dus(edges.size() + 1);
        vector<int> bad_edge(2);

        for (auto& r : edges)
        {
            if (dus.InSameSet(0, r[0]) && dus.InSameSet(0, r[1]))
            {
                bad_edge[0] = r[0];
                bad_edge[1] = r[1];
            }
            else
            {
                dus.Union(0, r[0]);
                dus.Union(0, r[1]);
            }
        }

        return bad_edge;
    }
};

int main()
{
    DisjointSet<string> dus;
    Output("size {}\nemptiness {}\n", dus.Size(), dus.Empty());
    dus.PushBack("cat");
    Output("size {}\nemptiness {}\n", dus.Size(), dus.Empty());

    dus.PushBack("cat owner");
    dus.PushBack("cat maid");
    dus.PushBack("cat girl");
    dus.PushBack("cat woman");

    Output("joined {}\n", dus.InSameSet(0, 3));
    dus.Union(0, 3);
    Output("joined {}\n", dus.InSameSet(0, 3));
    Output("joined {}\n", dus.InSameSet(3, 4));

    dus.Union(2, 4);
    dus.Union(2, 3);

    Output("joined {}\n", dus.InSameSet(3, 4));
    

}