#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <source_location>

//MSVC built-in debug flag
#if defined _DEBUG && (defined _WIN32 || defined _WIN64)

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




    class Pool
    {
    public:
        class AllocateInfo
        {
            const int num_;
            const std::source_location srce_;

        public:
            AllocateInfo(int num, const std::source_location srce = std::source_location::current()) : num_(num), srce_(srce) {}

            friend class Pool;
        };


    private:
        //address, byte size, location
        static std::unordered_map<void*, std::pair<size_t, std::string>> allocated_;

    public:

        template <typename T, typename... Args>
        static T* Allocate(const AllocateInfo& alloc_info, Args&&... args)
        {
            if (alloc_info.num_ <= 0)
            {
                throw std::runtime_error("Invalid heap size requested: " + std::to_string(static_cast<int>(alloc_info.num_ * sizeof(T))));
            }

            T* addr = new T[alloc_info.num_]{std::forward<Args>(args)...};
            allocated_.insert
            ({ 
                static_cast<void*>(addr), //address
                { alloc_info.num_* sizeof(T), std::string(alloc_info.srce_.function_name()) + std::to_string(alloc_info.srce_.line()) } //byte size + location
            });

            return addr;
        }

        template <typename T>
        static void Deallocate(T* addr, const std::source_location srce = std::source_location::current())
        {
            auto entry = allocated_.find(static_cast<void*>(addr));
            if (entry == allocated_.end())
            {
                throw std::runtime_error
                (
                    "Deallocated invalid address " + (std::stringstream() << addr).str() +
                    " at " + srce.function_name() + "_" + std::to_string(srce.line())
                );
            }

            allocated_.erase(entry);
            delete[] addr;
        }

        static void PrintPool()
        {
            std::cout << "Memory Pool Status:\n";
            for (const auto& entry : allocated_)
            {
                std::cout << entry.second.second << ": " << entry.second.first << " bytes allocated at " << entry.first << '\n';
            }
            std::cout << std::flush;
        }
    };
}
