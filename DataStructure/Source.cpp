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

    constexpr int kSZ = 10;

    vector<int> arr;
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

    vector<int> vec;
    for (int n : arr)
    {
        vec.push_back(n);
    }
    vec.clear();
    /*for (int n : vec)
    {
        cout << n << ' ';
    }*/
    
    timer.Stop();
    timer.Result();



    timer.Start();

    

    Stack<int> stk;
    for (int n : arr)
    {
        stk.Push(n);
    }

    Stack<int> stk1(stk);
    if (!stk.Empty()) std::cout << "Good copy constructor!\n";

    Stack<int> stk2(std::move(stk));
    if (stk.Empty()) std::cout << "Good move constructor!\n";

    for (int i = 0; i < stk.Size(); ++i)
    {
        cout << stk[i] << ' ';
    }
    std::cout << "\n\n";


    for (int i = 0; i < stk1.Size(); ++i)
    {
        cout << stk1[i] << ' ';
    }
    std::cout << "\n\n";


    for (int i = 0; i < stk2.Size(); ++i)
    {
        cout << stk2[i] << ' ';
    }
    std::cout << "\n\n";


    timer.Stop();
    timer.Result();
    std::cout << "\n\n";
}