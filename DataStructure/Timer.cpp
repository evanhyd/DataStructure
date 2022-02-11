#include "Timer.h"
#include <iostream>

void Timer::Start()
{
    begin_ = std::chrono::high_resolution_clock::now();
}
void Timer::Stop()
{
    end_ = std::chrono::high_resolution_clock::now();
}
void Timer::Result() const
{
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - begin_).count() << " ms\n";
}