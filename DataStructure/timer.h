#pragma once
#include <iostream>
#include <chrono>

namespace box {

  template <size_t maxRecordsSize = 256>
  class Timer {
    static_assert(maxRecordsSize > 0, "Timer must have a positive maximum records size");
    std::chrono::high_resolution_clock::time_point begin_{};
    std::chrono::high_resolution_clock::time_point records_[maxRecordsSize]{};
    size_t size = 0;

  public:
    void start() {
      begin_ = std::chrono::high_resolution_clock::now();
    }

    void record() {
      records_[size++] = std::chrono::high_resolution_clock::now();
    }

    void reset() {
      size = 0;
    }

    template <typename T = std::chrono::milliseconds>
    void dump(std::ostream& out = std::cout) const {
      for (size_t i = 0; i < size; ++i) {
        out << i << ": " << std::chrono::duration_cast<T>(records_[i] - begin_) << '\n';
      }
    }
  };
}
