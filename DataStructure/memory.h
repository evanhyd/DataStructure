#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <source_location>

//MSVC built-in debug flag
#if defined _DEBUG && (defined _WIN32 || defined _WIN64)
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif

namespace box
{
  //incompatible with address sanitizer
  inline void MemoryGuard() {
#if defined _DEBUG && (defined _WIN32 || defined _WIN64)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  }

  class DebugClass {
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
