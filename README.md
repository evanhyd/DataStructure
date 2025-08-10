# Flow C++ Data Structure Library

Flow is a modern C++ library providing efficient, high-performance data structures built with custom allocator support.

---

## 🚀 Features

- ⚡ **Allocator-aware containers**  
  Fully supports custom allocators (including polymorphic allocators) across all core data structures.

- 📦 **Modular & Lightweight**  
  Minimal dependencies with header only implementation.

- 🧠 **Custom Memory Resources**  
  Includes arena and monotonic memory resources to improve allocation performance and reduce fragmentation.

- ✅ **Extensive Unit Testing**  
  Uses GoogleTest to validate correctness and performance across all major components.

---

## 📘 Flow C++ Library Documentation

This site contains the auto-generated [API documentation](https://evanhyd.github.io/DataStructure/) for the Flow C++ Data Structure Library, built using Doxygen.

---

## 📚 Included Modules

| Class | Description |
|-------|-------------|
| **Container** ||
| [`Vector`](https://evanhyd.github.io/DataStructure/classflow_1_1_vector.html) | A dynamic array container with customizable allocator and growth strategy. Uses `PolymorphicAllocator` by default. |
| [`BinaryHeap`](https://evanhyd.github.io/DataStructure/classflow_1_1_binary_heap.html) | A binary min heap container with customizable allocator. Uses `PolymorphicAllocator` by default. |
| [`Tuple`](https://evanhyd.github.io/DataStructure/classflow_1_1_tuple.html) | A fixed-size container for heterogeneous values, similar to `std::tuple`, implemented with recursive inheritance. |
| **Memory Resource** ||
| [`PolymorphicAllocator`](https://evanhyd.github.io/DataStructure/classflow_1_1_polymorphic_allocator.html) | A polymorphic allocator that wraps around a non-owning memory resource. Memory allocation strategy is decided by the memory resource's implementation. |
| [`MemoryResource`](https://evanhyd.github.io/DataStructure/classflow_1_1_memory_resource.html) | An abstract interface used by `PolymorphicAllocator`. Responsible for raw memory allocation and deallocation. |
| &emsp;↳ [`DefaultMemoryResource`](https://evanhyd.github.io/DataStructure/classflow_1_1_default_memory_resource.html) | A default memory resource that wraps global `::operator new` and `::operator delete`. |
| &emsp;↳ [`ArenaMemoryResource`](https://evanhyd.github.io/DataStructure/classflow_1_1_arena_memory_resource.html) | A linear arena memory resource that allocates memory sequentially from a fixed buffer. Throws `std::bad_alloc` if there is insufficient space. |
| &emsp;↳ [`StackMemoryResource`](https://evanhyd.github.io/DataStructure/classflow_1_1_stack_memory_resource.html) | A stack-based memory resource that allocates memory in a LIFO order. Deallocation must happen in reverse order of allocation. |
| &emsp;↳ [`PoolMemoryResource`](https://evanhyd.github.io/DataStructure/classflow_1_1_pool_memory_resource.html) | A pool memory resource that manages fixed-size memory blocks from a pre-allocated buffer. Allocation must meet block size/alignment constraints. |

---
