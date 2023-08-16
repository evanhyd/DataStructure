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

  /*#define _DEBUG_MEMORY

  #ifdef  _DEBUG
  #ifdef _DEBUG_MEMORY
  void* operator new(std::size_t bytes, const std::source_location location);
  void operator delete(void* address, std::size_t size, const std::source_location location);

  class Pool {
    static std::unordered_map<void*, std::source_location> pool_;
    friend void* operator new(std::size_t bytes, const std::source_location location);
    friend void operator delete(void* address, std::size_t size, const std::source_location location);
  };
  #endif
  #endif*/





  //class Pool {

  //  //will replace this with explicit template deduction later
  //  class AllocInfo {
  //    const int num_;
  //    const std::source_location srce_;
  //  public:
  //    AllocInfo(const int num, const std::source_location srce = std::source_location::current()) : num_(num), srce_(srce) {}
  //    friend class Pool;
  //  };

  //  


  //public:
  //  /// <summary>
  //  /// Allocate objects on the pool. 
  //  /// </summary>
  //  /// <typeparam name="T"></typeparam>
  //  /// <typeparam name="...Args"></typeparam>
  //  /// <param name="alloc_info">: number of objects to allocate</param>
  //  /// <param name="...args">: constructor arguments for each object</param>
  //  /// <returns>the address of the allocated objects</returns>
  //  template <typename T, typename... Args>
  //  static T* Allocate(const AllocInfo& alloc_info, Args&&... args) {
  //    if (alloc_info.num_ <= 0) {
  //      throw std::runtime_error(
  //          "Invalid heap size requested: " +
  //          std::to_string(static_cast<int>(alloc_info.num_ * sizeof(T))));
  //    }

  //    // let's pray it doesn't throw
  //    T* addr = new T[alloc_info.num_]{ std::forward<Args>(args)... };

  //    pool_.insert({static_cast<void*>(addr),
  //                 std::string(alloc_info.srce_.function_name()) + "(" + std::to_string(alloc_info.srce_.line()) + ") " + 
  //                 std::string(typeid(T).name()) + "[" + std::to_string(alloc_info.num_) + "] " + 
  //                 std::to_string(alloc_info.num_ * sizeof(T)) + " bytes"});

  //    return addr;
  //  }

  //  /// <summary>
  //  /// Deallocate the address allocated by Pool::Allocate.
  //  /// </summary>
  //  /// <typeparam name="T"></typeparam>
  //  /// <param name="addr">: address to deallocate</param>
  //  template <typename T>
  //  static void Deallocate(T* addr, const std::source_location srce = std::source_location::current()) {
  //    auto entry = pool_.find(static_cast<void*>(addr));
  //    if (entry == pool_.end()) {
  //    throw std::runtime_error("Deallocated invalid address " +
  //                               (std::stringstream() << addr).str() + " at " +
  //                               srce.function_name() + "_" +
  //                               std::to_string(srce.line()));
  //    }
  //    pool_.erase(entry);
  //    delete[] addr;
  //  }

  //  /// <summary>
  //  /// Deallocate all the allocated resources.
  //  /// </summary>
  //  static void ClearPool() {
  //    for (const auto& entry : pool_) {
  //      delete[] entry.first;
  //    }
  //    pool_.clear();
  //  }

  //  /// <summary>
  //  /// Print the information about the allocated objects in the memory pool.
  //  /// </summary>
  //  static void PrintPool() {
  //    std::cout << "Memory Pool Status:\n";
  //    for (const auto& entry : pool_) {
  //      std::cout << "  " << entry.second << '\n';
  //    }
  //  }
  //};
}
