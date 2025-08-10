#pragma once
#include <chrono>
#include <format>
#include <string>
#include <vector>

namespace flow {

  /// @brief A simple timer to record timelapses. Uses steady_clock by default.
  template <typename ClockType = std::chrono::steady_clock>
  class Timer {
    using TimePoint = typename ClockType::time_point;
    TimePoint begin_{};
    std::vector<TimePoint> timepoints_{};

  public:
    /// @brief Constructs a Timer and reserves space for time points.
    /// @param reserveSize Number of time points to reserve space for.
    explicit constexpr Timer(std::size_t reserveSize = 8) {
      timepoints_.reserve(reserveSize);
    }

    /// @brief Returns the number of recorded time points.
    /// @return Count of recorded time points.
    constexpr std::size_t size() const {
      return timepoints_.size();
    }

    /// @brief Clears all recorded time points and resets the starting time.
    void reset() {
      timepoints_.clear();
      begin_ = ClockType::now();
    }

    /// @brief Records the current time point relative to the last reset.
    void record() {
      timepoints_.push_back(ClockType::now());
    }

    /// @brief Formats and returns a string showing all recorded durations since reset.
    /// @return Formatted string of all recorded time intervals.
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
