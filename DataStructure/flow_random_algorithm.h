#pragma once
#include <algorithm>
#include <concepts>
#include <random>

namespace flow {

  /// <summary>
  /// Produces a random number uniformly distributed on the closed interval [lower, upper].
  /// </summary>
  /// <returns>A random number in the closed interval.</returns>
  template <typename T>
  requires std::integral<T> || std::floating_point<T>
  T getRandomNumber(T lower, T upper) {
    static std::random_device device{};
    static std::mt19937_64 engine(device());
    if constexpr (std::integral<T>) {
      return std::uniform_int_distribution<T>(lower, upper)(engine);
    } else {
      return std::uniform_real_distribution<T>(lower, upper)(engine);
    }
  }

  /// <summary>
  /// Randomly shuffle the elements in the range [begin, end).
  /// </summary>
  template <std::random_access_iterator It>
  void shuffle(It begin, It end) {
    static std::random_device device{};
    static std::mt19937_64 engine(device());
    std::shuffle(begin, end, engine);
  }
}
