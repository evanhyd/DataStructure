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

#include "DynamicArray.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "PSDA.h"

#include "debug.h"

using namespace std;

using DC = DebugClass;




int main()
{
    //MEMORY_GUARD(); //address sanitizer is incompatiable with this

    short n = Read<short>();
    int m = Read<int>();
    long c = Read<long>();
    long long k = Read<long long>();

    unsigned short u = Read<unsigned short>();
    unsigned int v = Read<unsigned int>();
    unsigned long w = Read<unsigned long>();
    unsigned long long x = Read<unsigned long long>();
    
    Print("", n, m , c, k, u, v, w, x);

}