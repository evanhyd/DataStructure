#pragma once
#include <iostream>
#include <iomanip>
#include <cassert>
#include <source_location>



namespace cug::io
{
    void Input(auto&... rest)
    {
        (std::cin >> ... >> rest);
    }

    void Output(const char* hints);
    void Output(const char* hints, const auto& val, const auto&... param)
    {
        while (*hints)
        {
            if (*hints == '{')
            {
                //check unexpected end of string
                ++hints;
                assert(*hints);


                //copy the original cout format
                std::ios cout_state(nullptr);
                cout_state.copyfmt(std::cout);


                //check for width length
                int c_width = 0;
                if (*hints == ':')
                {
                    while (isdigit(*++hints))
                    {
                        c_width *= 10;
                        c_width += *hints - '0';
                    }
                }


                //check for float precision
                if (*hints == '.')
                {
                    int f_precision = 0;
                    while (isdigit(*++hints))
                    {
                        f_precision *= 10;
                        f_precision += *hints - '0';
                    }
                    std::cout << std::setprecision(f_precision);

                    //check for decimal fixed hint
                    if (*hints == 'f') std::cout << std::fixed;
                    ++hints;
                }


                //check for alignment
                if (*hints == 'l')
                {
                    std::cout << std::left;
                    ++hints;
                }
                else if (*hints == 'r')
                {
                    std::cout << std::right;
                    ++hints;
                }
                else if (*hints == 'i')
                {
                    std::cout << std::internal;
                    ++hints;
                }

                assert(*hints == '}');

                //print and then reset format
                std::cout << std::setw(c_width) << val;
                std::cout.copyfmt(cout_state);

                //recursively call to update the print argument
                return cug::io::Output(++hints, param...);
            }
            else
            {
                putchar(*hints);
            }

            ++hints;
        }
    }

#ifdef _DEBUG

    template <typename... Args>
    class Info
    {
    public:
        Info(const Args&... args, const std::source_location srce = std::source_location::current())
        {
            std::cout << '(' << srce.function_name() << '_' << srce.line() << ')';
            Output(args...);
        }
    };

    //require explicit template deduction
    template <typename... Args>
    Info(const Args&...)->Info<Args...>;


#else

    class Log { public: Log([[maybe_unused]]const auto&... args) {} };

#endif



    //not portable
#if defined _WIN32 || defined _WIN64
    inline char getchar_unlocked() { return static_cast<char>(_getchar_nolock()); }
    inline void putchar_unlocked(char c) { _putchar_nolock(c); }
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

    