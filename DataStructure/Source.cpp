#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <numeric>
#include <ctime>
#include <chrono>
#include "BinaryHeap.h"
#include "Timer.h"

using namespace std;

struct C
{
    bool operator()(const int& a, const int& b)
    {
        return a > b;
    }
};

int main()
{
    srand(time(NULL));

    constexpr int kSZ = 10;

    vector<int> arr;
    for (int i = 0; i < kSZ; ++i)
    {
        arr.push_back(rand() % kSZ + 1);
    }

    sort(arr.rbegin(), arr.rend());
    for (int n : arr)
    {
        cout << n << ' ';
    }
    std::cout << "\n\n";





    Timer timer;

    timer.Start();
    priority_queue<int> que;
    for (int n : arr)
    {
        que.push(n);
    }
    while (!que.empty())
    {
        int t = que.top();
        std::cout << t << ' ';
        que.pop();
    }
    timer.Stop();
    timer.Result();



    timer.Start();
    BinaryHeap<int, C> heap(arr);
    while (!heap.IsEmpty())
    {
        int t = heap.Top();
        cout << t << ' ';
        heap.Pop();
    }
    timer.Stop();
    timer.Result();
    std::cout << "\n\n";
}