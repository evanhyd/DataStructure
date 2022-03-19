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

using DC = DebugClass;

int main()
{
    DEBUG_MEMORY();

    vector<DC> vec;
    DynamicArray<DC> da;


    Timer t1;
    t1.Start();

    for (int i = 0; i < 99999999; ++i)
    {
        da.PushBack(DC(i * i));
    }

    t1.Stop();
    t1.Result();

    t1.Start();

    for (int i = 0; i < 99999999; ++i)
    {
        vec.push_back(DC(i * i));
    }

    t1.Stop();
    t1.Result();
    
}