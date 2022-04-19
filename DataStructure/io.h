#pragma once
#include <iostream>
#include <source_location>



namespace cug::io
{
    void Input(auto&... rest)
    {
        (std::cin >> ... >> rest);
    }
    void Output(const auto&... param)
    {
        (std::cout << ... << param);
    }

//#ifdef _DEBUG
//
//    void Log()
//    {
//        //std::cout << srce.function_name() << '_' << srce.line() << ':';
//        //cug::io::Print(log_info.sep_, rest...);
//    }
//#else
//    template <typename T>
//    void Log(const LogInfo<T>& log_info, const auto&... rest)
//    {
//        //empty
//    }
//#endif



    //not portable
#if defined _WIN32 || defined _WIN64
    inline char getchar_unlocked() { return static_cast<char>(_getchar_nolock()); }
#endif
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

