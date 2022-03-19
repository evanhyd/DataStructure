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
    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    arr.PushBack(10);
    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    arr.PushBack(20);
    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    arr.PushBack(20);
    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    /*arr.PushBack(20);
    arr.PushBack(10);
    arr.PushBack(20);*/

    /*cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    for (int i = 0; i < arr.Size(); ++i)
    {
        cout << arr[i] << ' ';
    }
    cout << '\n';*/


    /*arr.PopBack();


    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    for (int i = 0; i < arr.Size(); ++i)
    {
        cout << arr[i] << ' ';
    }
    cout << '\n';

    arr.Clear();



    cout << "cap: " << arr.Capacity() << '\n';
    cout << "size: " << arr.Size() << '\n';
    for (int i = 0; i < arr.Size(); ++i)
    {
        cout << arr[i] << ' ';
    }
    cout << '\n';*/

    
}