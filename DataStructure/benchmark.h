#pragma once
#include "flow_arena_memory_resource.h"
#include "flow_polymorphic_allocator.h"
#include "flow_timer.h"
#include "flow_vector.h"
#include "flow_random_algorithm.h"
#include <cstdint>
#include <memory>
#include <memory_resource>
#include <vector>

namespace benchmark {
  inline void benchmarkStdVector(size_t elementSize, size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "std::vector raw\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      vector<int64_t> vec{};

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector reserve\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      vector<int64_t> vec{};

      Timer timer{};
      timer.start();
      vec.reserve(elementSize);
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector raw arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<int64_t, PolymorphicAllocator<int64_t>> vec{ alloc };

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector reserve arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<int64_t, PolymorphicAllocator<int64_t>> vec{ alloc };

      Timer timer{};
      timer.start();
      vec.reserve(elementSize);
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowVector(size_t elementSize, size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "flow::Vector raw\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      Vector<int64_t> vec{};

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector reserve\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      Vector<int64_t> vec{};

      Timer timer{};
      timer.start();
      vec.reserve(elementSize);
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector raw arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<int64_t> vec{ alloc };

      Timer timer{};
      timer.start();

      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector reserve arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<int64_t> vec{ alloc };

      Timer timer{};
      timer.start();
      vec.reserve(elementSize);
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdVectorString(size_t elementSize, size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";
    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "std::vector raw\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      vector<string> vec{};

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector reserve\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      vector<string> vec{};
      vec.reserve(elementSize);

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector raw arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<string, PolymorphicAllocator<string>> vec{ alloc };

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector reserve arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<string, PolymorphicAllocator<string>> vec{ alloc };
      vec.reserve(elementSize);

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowVectorString(size_t elementSize, size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";
    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "flow::Vector raw\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      Vector<string> vec{};

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector reserve\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      Vector<string> vec{};
      vec.reserve(elementSize);

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector raw arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<string> vec{ alloc };

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector reserve arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<string> vec{ alloc };
      vec.reserve(elementSize);

      Timer timer{};
      timer.start();
      for (size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdSet(size_t elementSize, size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<byte[]> buffer = make_unique<byte[]>(elementSize * sizeof(int64_t) * 32);
    vector<int64_t> nums(elementSize);
    iota(nums.begin(), nums.end(), int64_t(0));
    flow::shuffle(nums.begin(), nums.end());

    cout << "std::set\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      set<int64_t, std::less<int64_t>> st{};

      Timer timer{};
      timer.start();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::set arena\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * sizeof(int64_t) * 32);
      PolymorphicAllocator<> alloc{ resource };
      set<int64_t, std::less<int64_t>, PolymorphicAllocator<int64_t>> st{ alloc };

      Timer timer{};
      timer.start();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::set monotonic\n";
    for (size_t bench = 0; bench < benchTime; ++bench) {
      std::pmr::monotonic_buffer_resource mbr{ buffer.get(), elementSize * sizeof(int64_t) * 32 };
      std::pmr::polymorphic_allocator<int64_t> pa{ &mbr };
      pmr::set<int64_t, std::less<int64_t>> st(pa);

      Timer timer{};
      timer.start();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }
}
