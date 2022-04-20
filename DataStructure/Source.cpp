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

#include "io.h"
#include "memory.h"
#include "time.h"

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


int main()
{
    /*std::ios cout_state(nullptr);
    cout_state.copyfmt(std::cout);

    double a = 3.1415926535;
    cout <<setw(10) << left << setprecision(2) << a << '\n';

    std::cout.copyfmt(cout_state);

    cout << a << '\n';*/

    string name;
    int age;
    double income;
    Input(name, age, income);
    Output("Hello I am {:20l}, {:.3f} years old, earn {}$ every day\n", name, age, income);

    Log("Stop");
}