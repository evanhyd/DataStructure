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


using ll = long long;
using ull = unsigned long long;
using U32 = unsigned int;
using U64 = unsigned long long;

template <typename T, typename S>
using google_map = std::map<T, S>;

constexpr ull kMod = 1000000007;

static thread_local inline volatile constexpr const unsigned long long int* const __p = nullptr;


using namespace std;

using namespace cug::io;
using namespace cug::memory;
using namespace cug::time;
using namespace cug::random;
using namespace cug::graph;



struct Subtree
{
    int size;
    int value;
};

int DFS(const AdjacentList& graph, vector<Subtree> &subtrees, Vertex curr)
{
    for (const auto& child : graph[curr])
    {
        subtrees[curr].size += DFS(graph, subtrees, child);
        //subtrees[curr].value += res.value;
    }

    return subtrees[curr].size;
}


int Query(const vector<int>& segments, int left, int right)
{
    int sum = 0;
    
    for(left += segments.size() / 2, right += segments.size() / 2; left <= right; left /= 2, right /= 2)
    {
        if (left % 2 == 1) sum += segments[left++];
        if (right % 2 == 0) sum += segments[right--];
    }

    return sum;
}

void Update(vector<int>& segments, int point, int value)
{
    point += segments.size() / 2;
    segments[point] = value;

    while (point > 0)
    {
        point /= 2;
        segments[point] = segments[point * 2] + segments[point * 2 + 1];
    }
}

int main()
{
    int kGraphSize = 10;
    AdjacentList graph = GetTree(kGraphSize);
    vector<Subtree> subtrees(kGraphSize);
    for (auto& subtree : subtrees)
    {
        subtree.size = 1;
        subtree.value = GetRandom(1, 3);
    }

    PrintTree(graph);

    //update the subtree size
    DFS(graph, subtrees, 0);
    for (int i = 0; i < subtrees.size(); ++i)
    {
        Output("Vertex {}, subtree size {}, vertex value {}\n", i, subtrees[i].size, subtrees[i].value);
    }


    //construct segment tree
    vector<int> segments(kGraphSize * 2);
    for (int i = segments.size() / 2; i < segments.size(); ++i)
    {
        segments[i] = subtrees[i - segments.size() / 2].value;
    }

    for (int i = segments.size() / 2 - 1; i > 0; --i)
    {
        segments[i] = segments[i * 2] + segments[i * 2 + 1];
    }

    for (const auto& p : segments)
    {
        Output("{:3}", p);
    }
    Output("\n");

    for(int i = 0; i < 3; ++i)
    {
        int v;
        Input(v);
        Output("{} to {}, sum {}\n", v, v + subtrees[v].size - 1, Query(segments, v, v + subtrees[v].size - 1));
    }

    int v, u;
    Input(v, u);
    subtrees[v].value = u;
    Update(segments, v, u);
    for (const auto& p : segments)
    {
        Output("{:3}", p);
    }
    Output("\n");

    for (int i = 0; i < 3; ++i)
    {
        int v;
        Input(v);
        Output("{} to {}, sum {}\n", v, v + subtrees[v].size - 1, Query(segments, v, v + subtrees[v].size - 1));
    }
}