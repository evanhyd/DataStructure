#pragma once
#include <chrono>
#include <format>
#include <vector>

namespace flow {
  class Timer {
    using ClockType = std::chrono::steady_clock;
    using TimePoint = ClockType::time_point;
    TimePoint begin_{};
    std::vector<TimePoint> timepoints_{};

  public:
    explicit constexpr Timer(std::size_t reserveSize = 8) {
      timepoints_.reserve(reserveSize);
    }

    constexpr std::size_t size() const {
      return timepoints_.size();
    }

    void start() {
      begin_ = ClockType::now();
    }

    void record() {
      timepoints_.push_back(ClockType::now());
    }

    void reset() {
      timepoints_.clear();
    }

    std::string toString() const {
      std::string str = std::format("Total record entries: {}\n", timepoints_.size());
      std::size_t i = 0;
      for (const auto& timepoint : timepoints_) {
        auto dur = timepoint - begin_;
        str += std::format("\t[{}] {}, {}, {}\n",
                           i,
                           std::chrono::duration_cast<std::chrono::seconds>(dur),
                           std::chrono::duration_cast<std::chrono::milliseconds>(dur),
                           std::chrono::duration_cast<std::chrono::microseconds>(dur));
        ++i;
      }
      return str;
    }
  };
}
