#pragma once
#include <iostream>

//MSVC built-in debug flag
#ifdef _DEBUG
#include <string>

//functions
#define LOG(sep, ...) cug::io::Print(sep, std::string(__FUNCTION__"_") + std::to_string(__LINE__), __VA_ARGS__)

#else
#define LOG(sep, ...)
#endif


//not portable
#if defined _WIN32 || defined _WIN64
inline char getchar_unlocked() { return static_cast<char>(_getchar_nolock()); }
#endif



namespace cug::io
{
    void Input(auto& curr)
    {
        std::cin >> curr;
    }
    void Input(auto& curr, auto&... rest)
    {
        std::cin >> curr;
        Input(rest...);
    }
    void Print([[maybe_unused]] const auto& sep, const auto& curr)
    {
        std::cout << curr << std::flush;
    }
    void Print(const auto& sep, const auto& curr, const auto&... rest)
    {
        std::cout << curr << sep;
        Print(sep, rest...);
    }


    template <std::signed_integral T>
    T Read()
    {
        T x; bool neg = false; char c{};
        do { c = getchar_unlocked(); if (c == '-') neg = true; } while (c < '0');
        for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0') {}
        return neg ? -x : x;
    }

    template <std::unsigned_integral T>
    T Read()
    {
        T x; char c{};
        do { c = getchar_unlocked(); } while (c < '0');
        for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
        return x;
    }

};

