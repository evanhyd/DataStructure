#include <iostream>
#include <numeric>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <execution>

#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <unordered_map>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Stack.h"
#include "ForwardList.h"
#include "BinaryHeap.h"
#include "Timer.h"

using namespace std;


constexpr int kSZ = 10;
using ValType = int;


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    srand(time(NULL));

    vector<ValType> arr;
    for (int i = 0; i < kSZ; ++i)
    {
        arr.push_back(rand() % (kSZ * 2) + 1);
        cout << arr[i] << ' ';
    }

    std::cout << '\n';

    ForwardList<ValType> lst;
    for (const auto& n : arr)
    {
        lst.PushFront(n);
    }
    lst.Reverse();

    auto node = lst.Head();
    while (node)
    {
        cout << node->val_ << ' ';
        node = node->next_;
    }

    std::cout << '\n';
    cout << "Front: " << lst.Front()<< '\n';

    auto lst1(std::move(lst));
    cout << "Front: " << lst1.Front() << '\n';
    if (!lst.Head()) cout << "Move constructor is good\n";
}