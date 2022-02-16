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
#include "Stack.h"
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


    Stack<ValType> arr;
    for (int i = 0; i < kSZ; ++i)
    {
        arr.PushBack(DebugClass());
    }

    Timer t1;
    t1.Start();

    forward_list<ValType> lst1;
    for (int i = 0; i < arr.Size(); ++i)
    {
        lst1.push_front(arr[i]);
    }
    

    t1.Stop();
    t1.Result();




    t1.Start();

    ForwardList<ValType> lst;
    for (int i = 0; i < arr.Size(); ++i)
    {
        lst.PushFront(arr[i]);
    }

    t1.Stop();
    t1.Result();
    
}