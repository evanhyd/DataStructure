#pragma once
#include <algorithm>
#include <random>
#include <type_traits>

namespace box {
  template <typename T>
  auto GetRandom(T lower_bound, T upper_bound) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "invalid parameter type");
    static std::random_device device;
    static std::mt19937_64 eng(device());

    if constexpr (std::is_integral_v<T>) {
      std::uniform_int_distribution<T> dis(lower_bound, upper_bound);
      return dis(eng);
    } else {
      std::uniform_real_distribution<T> dis(lower_bound, upper_bound);
      return dis(eng);
    }
  }

  template <typename T>
  auto Shuffle(T begin, T end) {
    static std::random_device device;
    static std::mt19937_64 eng(device());
    std::shuffle(begin, end, eng);
  }
}
