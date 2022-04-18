#pragma once
#include <chrono>

namespace cug::time
{
    class Timer
    {
        std::chrono::high_resolution_clock::time_point begin_;
        std::chrono::high_resolution_clock::time_point end_;

    public:
        void Start();
        void Stop();
        void Result() const;
    };
}


