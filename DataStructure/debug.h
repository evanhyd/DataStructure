#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>


#ifdef _DEBUG

//incompatiable with address sanitizer
#include <stdlib.h>
#include <crtdbg.h>
#include <string>
#define _CRTDBG_MAP_ALLOC

//functions
#define MEMORY_GUARD() (_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
#define LOG(sep, ...) Print(sep, std::string(__FUNCTION__) + std::string("-") + std::to_string(__LINE__), __VA_ARGS__);

#else

#define MEMORY_GUARD() 
#define LOG(sep, ...)

#endif


class DebugClass
{
    int id_;

public:
    explicit DebugClass();
    explicit DebugClass(int id);
    DebugClass(const DebugClass& rhs);
    DebugClass(DebugClass&& rhs) noexcept;
    ~DebugClass();

    DebugClass& operator=(const DebugClass& rhs);
    DebugClass& operator=(DebugClass&& rhs) noexcept;

    friend std::ostream& operator<<(std::ostream& output, const DebugClass& obj);

private:
    static int global_id_;
};


class Timer
{
    std::chrono::high_resolution_clock::time_point begin_;
    std::chrono::high_resolution_clock::time_point end_;

public:
    void Start();
    void Stop();
    void Result() const;
};



template <typename T>
void Print([[maybe_unused]] const std::string& sep, const T& curr)
{
    std::cout << curr << std::flush;
}

template <typename T, typename... Args>
void Print(const std::string& sep, const T& curr, const Args&... rest)
{
    std::cout << curr << sep;
    Print(sep, rest...);
}



//not portable
#if defined _WIN32 || defined _WIN64
char getchar_unlocked() { return char(_getchar_nolock()); }
#endif

template <typename T>
T Read()
{
    T x; bool neg = false; char c{};
    do { c = getchar_unlocked(); if (c == '-') neg = true; } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0') {}
    return neg ? -x : x;
}

template <>
unsigned short Read<unsigned short>()
{
    unsigned short x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

template <>
unsigned Read<unsigned>()
{
    unsigned x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

template <>
unsigned long Read<unsigned long>()
{
    unsigned long x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}

template <>
unsigned long long Read<unsigned long long>()
{
    unsigned long long x; char c{};
    do { c = getchar_unlocked(); } while (c < '0');
    for (x = c - '0'; '0' <= (c = getchar_unlocked()); x = (x << 3) + (x << 1) + c - '0');
    return x;
}