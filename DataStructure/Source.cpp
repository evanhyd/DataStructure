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
#include "Timer.h"

using namespace std;

constexpr int kSZ = 3;
using ValType = DebugClass;

int main()
{
    DEBUG_MEMORY();
    srand(time(NULL));

    DynamicArray<int> arr;

    DynamicArray<int> arr0(10);
    DynamicArray<int> arr1(10, 20);
    DynamicArray<int> arr3(arr0);
    arr3 = arr1;
}