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

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "DClass.h"
#include "Stack.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "Timer.h"

using namespace std;


constexpr int kSZ = 20;
using ValType = DClass;


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    srand(time(NULL));

    Stack<ValType> arr;
    for (int i = 0; i < kSZ; ++i)
    {
        arr.PushBack(ValType());
        //cout << arr[i] << ' ';
    }
    ValType v;
    arr.PushBack(v);
    arr.PopBack();


    return 1;

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