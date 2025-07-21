#pragma once
#include <chrono>
#include <format>
#include <vector>

namespace flow {
  /// <summary>
  /// A simple timer to record timelapses. Uses chrono::steady_clock by default.
  /// </summary>
  template <typename ClockType = std::chrono::steady_clock>
  class Timer {
    using TimePoint = ClockType::time_point;
    TimePoint begin_{};
    std::vector<TimePoint> timepoints_{};

  public:
    /// <summary>
    /// Constructs a Timer and reserves space for a given number of time points.
    /// </summary>
    explicit constexpr Timer(std::size_t reserveSize = 8) {
      timepoints_.reserve(reserveSize);
    }

    /// <summary>
    /// Returns the number of recorded time points.
    /// </summary>
    /// <returns>Count of time points recorded.</returns>
    constexpr std::size_t size() const {
      return timepoints_.size();
    }

    /// <summary>
    /// Clears all recorded time points and reset the starting time.
    /// </summary>
    void reset() {
      timepoints_.clear();
      begin_ = ClockType::now();
    }

    /// <summary>
    /// Records the current time point relative to the last call to start().
    /// </summary>
    void record() {
      timepoints_.push_back(ClockType::now());
    }

    /// <summary>
    /// Formats and returns a string showing all recorded durations since the timer resets.
    /// </summary>
    /// <returns>Formatted string of all recorded time intervals.</returns>
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
