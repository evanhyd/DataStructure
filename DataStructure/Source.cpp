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

int main()
{
    //DEBUG_MEMORY(); //address sanitizer is incompatiable with this

    PSDA<int> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    DynamicArray<int> arr1(arr.begin(), arr.end());
    for (auto n : arr1)
    {
        LOG("", "hello", '\n');
    }


}