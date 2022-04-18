#pragma once
#include <iostream>

//MSVC built-in debug flag
#ifdef _DEBUG

//incompatiable with address sanitizer
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

//functions
#define MEMORY_GUARD() (_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF))

#else
#define MEMORY_GUARD() 
#endif

namespace cug::memory
{
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

    std::ostream& operator<<(std::ostream& output, const DebugClass& obj);
}
