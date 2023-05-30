#pragma once
#include <random>

namespace cug::random
{
    template <std::integral T>
    auto GetRandom(const T lower_bound, const T upper_bound)
    {
        std::random_device device;
        std::mt19937_64 rng(device());
        std::uniform_int_distribution<T> CalcDist(lower_bound, upper_bound);
        return CalcDist(rng);
    }

    template <std::floating_point T>
    auto GetRandom(const T lower_bound, const T upper_bound)
    {
        std::random_device device;
        std::mt19937_64 rng(device());
        std::uniform_real_distribution<T> CalcDist(lower_bound, upper_bound);
        return CalcDist(rng);
    }
}

