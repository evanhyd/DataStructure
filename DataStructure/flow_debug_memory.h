#pragma once
#include <format>
#include <iostream>

// MSVC built-in debug flag
#if defined _DEBUG && (defined _WIN32 || defined _WIN64)
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif


namespace flow {
  // Incompatible with address sanitizer.
  inline void enableMemoryGuard() {
    #ifdef _CRTDBG_MAP_ALLOC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
  }

  class DebugClass {
    std::shared_ptr<std::size_t> copyCounter_;
    std::size_t copies_;
    std::size_t id_;

  public:
    DebugClass()
      : copyCounter_(std::make_shared<std::size_t>(0)),
        copies_(0),
        id_(globalId++) {
      std::cout << std::format("default_ctor id {}, copies {}\n", id_, copies_);
    }

    DebugClass(const DebugClass& rhs)
      : copyCounter_(rhs.copyCounter_),
        copies_(++(*copyCounter_)),
        id_(rhs.id_) {

      std::cout << std::format("copy_ctor id {}, copies {}\n", id_, copies_);
    }

    DebugClass(DebugClass&& rhs) noexcept
      : copyCounter_(std::move(rhs.copyCounter_)),
        copies_(rhs.copies_),
        id_(rhs.id_) {
      std::cout << std::format("move_ctor id {}, copies {}\n", id_, copies_);
    }

    ~DebugClass() {
      std::cout << std::format("dtor id {}, copies {}\n", id_, copies_);
    }

    DebugClass& operator=(const DebugClass& rhs) {
      copyCounter_ = rhs.copyCounter_;
      copies_ = ++(*copyCounter_);
      id_ = rhs.id_;
      std::cout << std::format("copy_op id {}, copies {}\n", id_, copies_);
    }

    DebugClass& operator=(DebugClass&& rhs) noexcept {
      copyCounter_ = std::move(rhs.copyCounter_);
      copies_ = rhs.copies_;
      id_ = rhs.id_;
      std::cout << std::format("move_op id {}, copies {}\n", id_, copies_);
    }

  private:
    static inline std::size_t globalId = 0;

    friend std::ostream& operator<<(std::ostream& out, const DebugClass& obj) {
      out << std::format("{{id: {}, copies: {}}}", obj.id_, obj.copies_);
      return out;
    }
  };
}
