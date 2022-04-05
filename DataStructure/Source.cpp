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

    short n;
    int m;
    long c;
    long long k;

    Input(n, m, c, k);
    Print(", ", n, m, c, k);
}