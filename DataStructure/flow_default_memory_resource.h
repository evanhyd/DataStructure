#pragma once
#include "flow_memory_resource.h"

namespace flow {

  /// @brief
  /// A default memory resource that wraps global ::operator new and ::operator delete.
  class DefaultMemoryResource : public MemoryResource {
  public:
    static DefaultMemoryResource& getResource() {
      static DefaultMemoryResource instance{};
      return instance;
    }

  private:
    virtual void* allocateImp(std::size_t bytes, std::size_t alignment) {
      return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
    }

    virtual void deallocateImp(void* address, [[maybe_unused]] std::size_t bytes, std::size_t alignment) {
      ::operator delete(address, static_cast<std::align_val_t>(alignment));
    }
  };
}
