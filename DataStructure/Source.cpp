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

class Matrix
{
public:
    ull m_[4];

    Matrix operator*(const Matrix& other)
    {
        Matrix copy{};
        copy.m_[0] = (m_[0] * other.m_[0] % kMod + m_[1] * other.m_[2] % kMod) % kMod;
        copy.m_[1] = (m_[0] * other.m_[1] % kMod + m_[1] * other.m_[3] % kMod) % kMod;
        copy.m_[2] = (m_[2] * other.m_[0] % kMod + m_[3] * other.m_[2] % kMod) % kMod;
        copy.m_[3] = (m_[2] * other.m_[1] % kMod + m_[3] * other.m_[3] % kMod) % kMod;

        return copy;
    }
};

struct BigInt
{
    string num;

    BigInt& operator/=(int a)
    {
        string quotient;
        int remainder = 0;

        while (num.size())
        {
            if (num.back() == '0') num.pop_back();
            else break;
        }

        for (int i = num.size() - 1; i >= 0; --i)
        {
            remainder = remainder * 10 + num[i] - '0';
            num[i] = remainder / 2 + '0';
            remainder = remainder % 2;
        }

        while (num.size())
        {
            if (num.back() == '0') num.pop_back();
            else break;
        }

        return *this;
    }

    bool IsZero() const
    {
        return num.empty();
    }

    bool IsOdd() const
    {
        return num.front() % 2;
    }
};

int main()
{
    cin.tie(nullptr)->sync_with_stdio(false);



    for (int kMod = 2; ; ++kMod)
    {
        int n = 0;
        int f0 = 0, f1 = 1;
        while (++n)
        {
            int f2 = (f0 + f1) % kMod;
            f0 = f1;
            f1 = f2;
            if (f0 == 0 && f1 == 1) break;
        }

        Log("{}\n", n);
    }
    


    return 0;

    BigInt N;


    cin >> N.num;
    reverse(N.num.begin(), N.num.end());

    Matrix base{ 1, 1, 1, 0 };
    Matrix product{ 1, 0, 0, 1 };

    while (!N.IsZero())
    {
        if (N.IsOdd()) product = product * base;

        N /= 2;
        base = base * base;
    }

    cout << product.m_[1] << '\n';
}