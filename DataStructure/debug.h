#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>

#define DEBUG

#ifdef DEBUG

//incompatiable with address sanitizer
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define MEMORY_GUARD() (_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
#define LOG(sep, ...) std::cout <<__func__<<"-"<<__LINE__<<":"; Print(sep, __VA_ARGS__);

#elif

#define DEBUG_MEMORY() 
#define DEBUG_LOG(obj, debug_mode)

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