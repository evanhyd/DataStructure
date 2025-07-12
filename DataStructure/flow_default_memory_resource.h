#pragma once
#include "flow_memory_resource.h"

namespace flow {
  class DefaultMemoryResource : public MemoryResource {
  public:
    static DefaultMemoryResource& getResource() {
      static DefaultMemoryResource instance{};
      return instance;
    }

  private:
    virtual void* allocateImp(size_t bytes, size_t alignment) {
      return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
    }

    virtual void deallocateImp(void* address, [[maybe_unused]] size_t bytes, size_t alignment) {
      ::operator delete(address, static_cast<std::align_val_t>(alignment));
    }
  };
}
