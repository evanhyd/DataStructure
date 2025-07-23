#pragma once
#include "flow_arena_memory_resource.h"
#include "flow_binary_heap.h"
#include "flow_polymorphic_allocator.h"
#include "flow_random_algorithm.h"
#include "flow_timer.h"
#include "flow_vector.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

namespace benchmark {
  inline void benchmarkStdVectorInt64(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "std::vector<int64_t> raw\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      vector<int64_t> vec{};

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<int64_t> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      vector<int64_t> vec{};

      Timer timer{};
      timer.reset();
      vec.reserve(elementSize);
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<int64_t> raw arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<int64_t, PolymorphicAllocator<int64_t>> vec{ alloc };

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<int64_t> reserve arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<int64_t, PolymorphicAllocator<int64_t>> vec{ alloc };

      Timer timer{};
      timer.reset();
      vec.reserve(elementSize);
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowVectorInt64(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "flow::Vector<int64_t> raw\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      Vector<int64_t> vec{};

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<int64_t> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      Vector<int64_t> vec{};

      Timer timer{};
      timer.reset();
      vec.reserve(elementSize);
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<int64_t> raw arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<int64_t> vec{ alloc };

      Timer timer{};
      timer.reset();

      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<int64_t> reserve arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<int64_t> vec{ alloc };

      Timer timer{};
      timer.reset();
      vec.reserve(elementSize);
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(i);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdVectorString(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";
    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "std::vector<std::string> raw\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      vector<string> vec{};

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<std::string> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      vector<string> vec{};
      vec.reserve(elementSize);

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<std::string> raw arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<string, PolymorphicAllocator<string>> vec{ alloc };

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::vector<std::string> reserve arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      vector<string, PolymorphicAllocator<string>> vec{ alloc };
      vec.reserve(elementSize);

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.push_back(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowVectorString(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";
    unique_ptr<int64_t[]> buffer = make_unique<int64_t[]>(elementSize * 32);

    cout << "flow::Vector<std::string> raw\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      Vector<string> vec{};

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<std::string> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      Vector<string> vec{};
      vec.reserve(elementSize);

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<std::string> raw arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<string> vec{ alloc };

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::Vector<std::string> reserve arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * 32 * sizeof(int64_t));
      PolymorphicAllocator<> alloc{ resource };
      Vector<string> vec{ alloc };
      vec.reserve(elementSize);

      Timer timer{};
      timer.reset();
      for (std::size_t i = 0; i < elementSize; ++i) {
        vec.pushBack(sample);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdSet(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    unique_ptr<byte[]> buffer = make_unique<byte[]>(elementSize * sizeof(int64_t) * 32);
    vector<int64_t> nums(elementSize);
    iota(nums.begin(), nums.end(), int64_t(0));
    flow::shuffle(nums.begin(), nums.end());

    cout << "std::set<int64_t>\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      set<int64_t, std::less<int64_t>> st{};

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::set<int64_t> arena\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      ArenaMemoryResource resource(buffer.get(), elementSize * sizeof(int64_t) * 32);
      PolymorphicAllocator<> alloc{ resource };
      set<int64_t, std::less<int64_t>, PolymorphicAllocator<int64_t>> st{ alloc };

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "std::set<int64_t> monotonic\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      std::pmr::monotonic_buffer_resource mbr{ buffer.get(), elementSize * sizeof(int64_t) * 32 };
      std::pmr::polymorphic_allocator<int64_t> pa{ &mbr };
      pmr::set<int64_t, std::less<int64_t>> st(pa);

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        st.insert(n);
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdPriorityQueueInt64(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    vector<int64_t> nums(elementSize);
    iota(nums.begin(), nums.end(), int64_t(0));
    flow::shuffle(nums.begin(), nums.end());

    cout << "std::priority_queue<int64_t>\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      priority_queue<int64_t> pq;

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        pq.push(n);
      }
      while (!pq.empty()) {
        pq.pop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowBinaryHeapInt64(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    vector<int64_t> nums(elementSize);
    iota(nums.begin(), nums.end(), int64_t(0));
    flow::shuffle(nums.begin(), nums.end());

    cout << "flow::BinaryHeap<int64_t>\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      BinaryHeap<int64_t> bh;

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        bh.push(n);
      }
      while (!bh.empty()) {
        bh.drop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::BinaryHeap<int64_t> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      BinaryHeap<int64_t> bh;
      bh.reserve(nums.size());

      Timer timer{};
      timer.reset();
      for (auto n : nums) {
        bh.push(n);
      }
      while (!bh.empty()) {
        bh.drop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkStdPriorityQueueString(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";

    vector<string> strings(elementSize);
    for (std::size_t i = 0; i < elementSize; ++i) {
      strings[i] = sample + " " + to_string(i);
    }
    flow::shuffle(strings.begin(), strings.end());

    cout << "std::priority_queue<std::string>\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      priority_queue<string> pq;

      Timer timer{};
      timer.reset();
      for (auto& s : strings) {
        pq.push(s);
      }
      while (!pq.empty()) {
        pq.pop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }

  inline void benchmarkFlowBinaryHeapString(std::size_t elementSize, std::size_t benchTime) {
    using namespace std;
    using namespace flow;

    const string sample = "the quick brown fox jumps over the lazy dog";

    vector<string> strings(elementSize);
    for (std::size_t i = 0; i < elementSize; ++i) {
      strings[i] = sample + " " + to_string(i);
    }
    flow::shuffle(strings.begin(), strings.end());

    cout << "flow::BinaryHeap<std::string>\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      BinaryHeap<string> bh;

      Timer timer{};
      timer.reset();
      for (auto& s : strings) {
        bh.push(s);
      }
      while (!bh.empty()) {
        bh.drop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }

    cout << "flow::BinaryHeap<std::string> reserve\n";
    for (std::size_t bench = 0; bench < benchTime; ++bench) {
      BinaryHeap<string> bh;
      bh.reserve(strings.size());

      Timer timer{};
      timer.reset();
      for (auto& s : strings) {
        bh.push(s);
      }
      while (!bh.empty()) {
        bh.drop();
      }
      timer.record();
      cout << timer.toString() << '\n';
    }
  }
}
