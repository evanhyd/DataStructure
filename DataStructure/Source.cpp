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
#include <thread>

#include <bit>
#include <any>
#include <cmath>
#include <ctime>
#include <climits>
#include <cassert>
#include <numeric>
#include <chrono>
#include <random>
#include <memory>
#include <algorithm>
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

#endif // !ONLINE_JUDGE


using ll = long long;
using ull = unsigned long long;

template <typename T, typename S>
using google_map = std::map<T, S>;

constexpr ull kMod = 1000000007;

static thread_local inline volatile constexpr const unsigned long long int* const __p = nullptr;


using namespace std;

#ifndef ONLINE_JUDGE

using namespace cug::io;
using namespace cug::memory;
using namespace cug::time;
using namespace cug::random;
using namespace cug::graph;

#endif // !ONLINE_JUDGE


#ifdef ONLINE_JUDGE

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

#endif




void DFS(const vector<vector<int>>& graph, const vector<int>& depths, vector<int>& v_to_i, vector<pair<int, int>>& segtree, int curr)
{
    segtree.push_back({ curr, depths[curr] });
    v_to_i[curr] = int(segtree.size()) - 1;

    for (int child : graph[curr])
    {
        DFS(graph, depths, v_to_i, segtree, child);
    }

    segtree.push_back({ curr, depths[curr] });
    v_to_i[curr] = int(segtree.size()) - 1;
}

int Query(const vector<pair<int, int>>& segtree, int l, int r)
{
    pair<int, int> res{0, INT_MAX};

    l += segtree.size() / 2;
    r += segtree.size() / 2;

    for (; l <= r; l /= 2, r /= 2)
    {
        if (l % 2 == 1)
        {
            if (segtree[l].second < res.second) res= segtree[l];
            ++l;
        }

        if (r % 2 == 0)
        {
            if (segtree[r].second < res.second) res = segtree[r];
            --r;
        }
    }

    return res.first;
}

int main()
{
    cin.tie(nullptr)->sync_with_stdio(false);

    int N, Q;
    cin >> N >> Q;

    vector<vector<int>> G(N); //adjacent list for DFS
    vector<int> depths(N); //depth of vertex
    vector<int> v_to_i(N); //convert vertex to segtree index
    vector<pair<int, int>> segtree; //segtree for finding min depth


    //create adjacent list
    for (int c = 1; c < N; ++c)
    {
        int p; cin >> p; p -= 1;

        G[p].push_back(c);
        depths[c] = depths[p] + 1;
    }

    PrintTree(G);

    //obtain euler tour DFS order
    DFS(G, depths, v_to_i, segtree, 0);

    //fixed the index after expanding segtree
    for (auto& n : v_to_i)
    {
        n += segtree.size();
    }

    //build tree
    segtree.insert(segtree.begin(), int(segtree.size()), {});
    for (int i = segtree.size() / 2 - 1; i > 0; --i)
    {
        if (segtree[i * 2] <= segtree[i * 2 + 1]) segtree[i] = segtree[i * 2];
        else segtree[i] = segtree[i * 2 + 1];
    }




    //query
    while (Q--)
    {
        int l, r;
        cin >> l >> r;
        --l; --r;

        cout << Query(segtree, min(depths[l], depths[r]), max(depths[l], depths[r])) + 1 << '\n';
    }
}