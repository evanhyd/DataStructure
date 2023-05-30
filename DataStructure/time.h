#pragma once
#include <iostream>
#include <chrono>

namespace box {
  class Timer {
    std::chrono::high_resolution_clock::time_point _begin;
    std::chrono::high_resolution_clock::time_point _end;

   public:
    void Start();
    void Stop();
    
    template <typename T = std::chrono::milliseconds>
    void Print(std::ostream& output = std::cout) const {
      output << std::chrono::duration_cast<T>(_end - _begin).count();
    }
  };
}


