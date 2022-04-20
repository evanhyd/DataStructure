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

#include "io.h"
#include "memory.h"
#include "time.h"
#include "random.h"

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


struct Sum
{
    ull operator()(const ull &a, const ull &b)
    {
        return lcm(a, b);
    }

    static constexpr ull init_value = INT_MAX;
};

int main()
{
    vector<ull> vec(16);
    for_each(vec.begin(), vec.end(), [](auto& val) {val = GetRandom(1ull, 10ull); });
    SegmentTree<ull, Sum> tree(vec);

    while (true)
    {
        tree.PrintTree(4);
        int opt;
        Output("#1. Query\n#2. Update\n");
        Input(opt);

        if (opt == 1)
        {
            int a, b;
            Input(a, b);
            Output("Query Result: {}\n", tree.Query(a, b));
        }
        else
        {
            int a, b;
            Input(a, b);
            tree.Update(a, b);
        }
    }

}