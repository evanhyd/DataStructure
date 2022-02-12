#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <numeric>
#include <ctime>
#include <chrono>

#include "Stack.h"
#include "Timer.h"

using namespace std;

int main()
{
    srand(time(NULL));

    constexpr int kSZ = 100000000;
    using ValType = double;

    vector<ValType> arr;
    for (int i = 0; i < kSZ; ++i)
    {
        arr.push_back(rand() % kSZ + 1);
    }

    /*sort(arr.rbegin(), arr.rend());
    for (int n : arr)
    {
        cout << n << ' ';
    }
    std::cout << "\n\n";*/


    Timer timer;
    timer.Start();

    vector<ValType> vec;
    for (auto n : arr)
    {
        vec.push_back(n);
    }
    /*for (int n : vec)
    {
        cout << n << ' ';
    }*/
    
    timer.Stop();
    timer.Result();



    timer.Start();

    Stack<ValType> stk;
    for (auto n : arr)
    {
        stk.Push(n);
    }

    timer.Stop();
    timer.Result();
    std::cout << "\n\n";
}