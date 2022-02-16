#pragma once
#include <iostream>
#include <iomanip>

#define DEBUG

#ifdef DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_MEMORY() (_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
#define DEBUG_LOG(obj, debug_mode) if (debug_mode) std::cout <<__func__<<" -> "<<__LINE__<<": "<< (obj) << std::endl;

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
};