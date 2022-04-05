#include <iostream>
#include <numeric>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <execution>

#include <string>
#include <deque>
#include <queue>
#include <forward_list>
#include <map>
#include <unordered_map>

#include "debug.h"
#include "DynamicArray.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "PSDA.h"

using namespace std;

using DC = DebugClass;

//not portable
#if defined _WIN32 || defined _WIN64
char getchar_unlocked() { return char(_getchar_nolock()); }
#endif

template <typename T>
T Read()
{
    T x; bool neg = false; char c{};
    do { c = getchar_unlocked(); if (c == '-') neg = true; } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0') {}
    return neg ? -x : x;
}

template <>
unsigned short Read<unsigned short>()
{
    unsigned short x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

template <>
unsigned Read<unsigned>()
{
    unsigned x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

template <>
unsigned long long Read<unsigned long long>()
{
    unsigned long long x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

int main()
{
    //MEMORY_GUARD(); //address sanitizer is incompatiable with this


    int n = Read<int>();
    long long k = Read<long long>();
    unsigned short m = Read<unsigned short>();
    unsigned z = Read<unsigned>();
    unsigned long long p = Read<unsigned long long>();
    
    Print(",", n, k, m ,z , p);

}