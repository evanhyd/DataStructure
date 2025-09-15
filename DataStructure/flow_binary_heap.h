#pragma once
#include "flow_polymorphic_allocator.h"
#include "flow_vector.h"
#include <cassert>
#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace flow {

  /// @brief A binary min-heap container. Supports custom comparator and allocator.
  template <typename T, typename Compare = std::less<T>, typename Allocator = PolymorphicAllocator<>>
  class BinaryHeap {
  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using difference_type = std::ptrdiff_t;
    using container_type = Vector<T, Allocator>;
    using allocator_type = container_type::allocator_type;

  private:
    container_type data_;
    Compare comparator_;

  public:
    /// @brief Constructs an empty heap with default comparator and allocator.
    /// Comparator must be default initializable.
    constexpr BinaryHeap() 
      noexcept(noexcept(container_type()) && noexcept(Compare()))
      requires std::default_initializable<Compare>
      : data_(), comparator_() {
    }

    /// @brief Constructs an empty heap with a specific allocator.
    /// Comparator must be default initializable.
    explicit constexpr BinaryHeap(const allocator_type& allocator)
      noexcept(noexcept(container_type(allocator)) && noexcept(Compare()))
      requires std::default_initializable<Compare>
      : data_(allocator), comparator_() {
    }

    /// @brief Constructs an empty heap with a specific allocator.
    /// @param comparator Comparator.
    /// @param allocator Memory allocator.
    explicit constexpr BinaryHeap(Compare comparator, const allocator_type& allocator = {})
      noexcept(noexcept(container_type(allocator)) && noexcept(Compare(std::move(comparator))))
      requires !std::default_initializable<Compare>
      : data_(allocator), comparator_(std::move(comparator)) {
    }

    constexpr BinaryHeap(const BinaryHeap&) = default;
    constexpr BinaryHeap(const BinaryHeap& rhs, const allocator_type& allocator)
      : data_(rhs.data_, allocator), comparator_(rhs.comparator_) {
    }

    constexpr BinaryHeap(BinaryHeap&&) = default;
    constexpr BinaryHeap(BinaryHeap&& rhs, const allocator_type& allocator)
      noexcept(noexcept(container_type(std::move(rhs.data_), allocator)) && noexcept(Compare(std::move(rhs.comparator_))))
      : data_(std::move(rhs.data_), allocator), comparator_(std::move(rhs.comparator_)) {
    }

    /// @brief Iterator range constructor.
    /// @param first Iterator to first element.
    /// @param last Iterator to one-past-last element.
    /// @param comp Comparator.
    /// @param alloc Allocator.
    template <std::input_iterator It>
    explicit constexpr BinaryHeap(It first, It last, Compare comparator = {}, const allocator_type& allocator = {})
      : data_(first, last, allocator), comparator_(std::move(comparator)) {
      heapify();
    }

    /// @brief Initializer list range constructor.
    /// @param list Initializer list.
    /// @param comp Comparator.
    /// @param alloc Allocator.
    explicit constexpr BinaryHeap(std::initializer_list<T> list, Compare comparator = {}, const allocator_type& allocator = {})
      : BinaryHeap(list.begin(), list.end(), std::move(comparator), allocator) {
    }

    ~BinaryHeap() = default;

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return data_.get_allocator();
    }

    // Accessor.

    /// @brief Return the heap size.
    /// @return The heap size. 
    constexpr std::size_t size() const noexcept {
      return data_.size();
    }

    /// @brief Return the heap capacity.
    /// @return The heap capacity.
    constexpr std::size_t capacity() const noexcept {
      return data_.capacity();
    }

    /// @brief Return true if empty.
    /// @return true if empty, otherwise false.
    constexpr bool empty() const noexcept {
      return data_.empty();
    }

    /// @brief Return the min element.
    /// @return The min element.
    const T& top() const noexcept {
      assert(!data_.empty() && "call top on an empty heap");
      return data_.front();
    }

    /// @brief Clear the heap.
    void clear() noexcept {
      data_.clear();
    }

    /// @brief Reserve the heap capacity.
    /// @param capacity 
    void reserve(std::size_t capacity) {
      data_.reserve(capacity);
    }

    /// @brief Push the value to the heap.
    /// @param value 
    void push(const T& value) {
      emplace(value);
    }

    /// @brief Push the value to the heap.
    /// @param value 
    void push(T&& value) {
      emplace(std::move(value));
    }

    /// @brief Construct the value inplace in the heap.
    /// @param ...args Constructor arguments.
    template <typename ...Args>
    void emplace(Args&&... args) {
      data_.emplaceBack(std::forward<Args>(args)...);
      T value = std::move(data_.back());
      fixUp(data_.size() - 1, 0, std::move(value));
    }

    /// @brief Drop the min element.
    void drop() 
      noexcept(std::is_nothrow_move_assignable_v<T>) {
      assert(!data_.empty() && "call drop on an empty heap");
      std::size_t index = fixDownWithHole(0);
      fixUp(index, 0, std::move(data_.back()));
      data_.popBack();
    }

    /// @brief Pop the min element.
    /// @return The min element.
    T pop() 
      noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {
      assert(!data_.empty() && "call pop on an empty heap");
      T value = std::move(data_.front());
      drop();
      return value;
    }

  private:
    // Fix down by repeatedly choosing the less child.
    // Return the index of the hole at the bottom.
    std::size_t fixDownWithHole(std::size_t index)
      noexcept(std::is_nothrow_move_assignable_v<T>) {
      for (;;) {
        size_t child = getLeftChild(index);
        if (child >= data_.size()) {
          break;
        }

        // Select the less child.
        std::size_t rightChild = child + 1;
        if (rightChild < data_.size() && comparator_(data_[rightChild], data_[child])) {
          child = rightChild;
        }
        data_[index] = std::move(data_[child]);
        index = child;
      }
      return index;
    }

    // Fix up the value.
    void fixUp(std::size_t index, std::size_t top, T&& value) 
      noexcept(std::is_nothrow_move_assignable_v<T>) {
      while (index > top) {
        std::size_t parent = getParent(index);
        if (comparator_(value, data_[parent])) {
          data_[index] = std::move(data_[parent]);
          index = parent;
        } else {
          break;
        }
      }
      data_[index] = std::move(value);
    }

    void heapify() 
      noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>) {
      if (data_.size() < 2) {
        return;
      }
      for (std::size_t i = data_.size() / 2; i-- > 0 ;) {
        T value = std::move(data_[i]);
        std::size_t hole = fixDownWithHole(i);
        fixUp(hole, i, std::move(value));
      }
    }

    static constexpr bool hasParent(std::size_t i) noexcept {
      return i > 0;
    }

    static constexpr std::size_t getParent(std::size_t i) noexcept {
      assert(i > 0 && "calling parent on the root node");
      return (i - 1) / 2;
    }

    static constexpr std::size_t getLeftChild(std::size_t i) noexcept {
      return i * 2 + 1;
    }
  };
}
