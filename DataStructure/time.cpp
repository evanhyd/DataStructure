#include "time.h"
#include <iostream>

//TIMER
using namespace std::chrono;
using namespace cug::time;

void Timer::Start()
{
    begin_ = high_resolution_clock::now();
}
void Timer::Stop()
{
    end_ = high_resolution_clock::now();
}
void Timer::Result() const
{
    std::cout << duration_cast<milliseconds>(end_ - begin_).count() << " ms\n";
}