#pragma once
#include <iostream>
#include <iomanip>

#define DEBUG

#ifdef DEBUG

//incompatiable with address sanitizer
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define DEBUG_MEMORY() (_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
#define DEBUG_LOG(obj, debug_mode) if (debug_mode) std::cout <<__func__<<" -> "<<__LINE__<<": "<< (obj) << std::endl;

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