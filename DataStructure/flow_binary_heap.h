#pragma once
#include "flow_polymorphic_allocator.h"
#include "flow_vector.h"
#include <cassert>
#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace flow {

  // A min heap.
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
    using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<T>;

  private:
    using container_type = Vector<T, Allocator>;
    container_type data_;
    Compare comparator_;

  public:
    // Comparator is default initializable.
    // T t, T t(), T t{}
    // Default constructor.
    constexpr BinaryHeap() 
      noexcept(noexcept(container_type()) && noexcept(Compare()))
      requires std::default_initializable<Compare>
      : data_(), comparator_() {
    }

    // Extended default constructor.
    explicit constexpr BinaryHeap(const allocator_type& allocator)
      noexcept(noexcept(container_type(allocator)) && noexcept(Compare()))
      requires std::default_initializable<Compare>
      : data_(allocator), comparator_() {
    }

    // Default constructor with non-default-initializable comparator.
     explicit constexpr BinaryHeap(Compare comparator, const allocator_type& allocator = {})
      noexcept(noexcept(container_type(allocator)) && noexcept(Compare(std::move(comparator))))
      requires !std::default_initializable<Compare>
      : data_(allocator), comparator_(std::move(comparator)) {
    }

    // Copy constructor and move constructor.
    constexpr BinaryHeap(const BinaryHeap&) = default;
    constexpr BinaryHeap(const BinaryHeap& rhs, const allocator_type& allocator)
      : data_(rhs.data_, allocator), comparator_(rhs.comparator_) {
    }

    constexpr BinaryHeap(BinaryHeap&&) = default;
    constexpr BinaryHeap(BinaryHeap&& rhs, const allocator_type& allocator)
      noexcept(noexcept(container_type(std::move(rhs.data_), allocator)) && noexcept(Compare(std::move(rhs.comparator_))))
      : data_(std::move(rhs.data_), allocator), comparator_(std::move(rhs.comparator_)) {
    }

    // Iterator constructor.
    template <std::input_iterator It>
    constexpr BinaryHeap(It first, It last, const allocator_type& allocator = {})
      : data_(first, last, allocator), comparator_() {
      heapify();
    }

    // Iterator constructor with non-default-initializable comaprator
    template <std::input_iterator It>
    constexpr BinaryHeap(It first, It last, Compare comparator, const allocator_type& allocator = {})
      : data_(first, last, allocator), comparator_(std::move(comparator)) {
      heapify();
    }

    ~BinaryHeap() = default;

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return data_.get_allocator();
    }

    // Accessor.
    constexpr std::size_t size() const noexcept {
      return data_.size();
    }

    constexpr std::size_t capacity() const noexcept {
      return data_.capacity();
    }

    constexpr bool empty() const noexcept {
      return data_.empty();
    }

    const T& top() const noexcept {
      assert(!data_.empty() && "call top on an empty heap");
      return data_.front();
    }

    // Mutator.
    void clear() noexcept {
      data_.clear();
    }

    void reserve(std::size_t capacity) {
      data_.reserve(capacity);
    }

    template <typename ...Args>
    void emplace(Args&&... args) {
      data_.emplaceBack(std::forward<Args>(args)...);
      fixUp(data_.size() - 1);
    }

    void push(const T& value) {
      emplace(value);
    }

    void push(T&& value) {
      emplace(std::move(value));
    }

    void drop() 
      noexcept(std::is_nothrow_move_assignable_v<T>) {
      assert(!data_.empty() && "call drop on an empty heap");
      data_.front() = std::move(data_.back());
      data_.popBack();
      fixDown(0);
    }

    T pop() 
      noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>) {
      assert(!data_.empty() && "call pop on an empty heap");
      T popped = std::exchange(data_.front(), std::move(data_.back()));
      data_.popBack();
      fixDown(0);
      return popped;
    }

  private:
    void heapify() 
      noexcept(std::is_nothrow_swappable_v<T>) {
      if (data_.size() >= 2) {
        for (std::size_t i = getParent(data_.size() - 1); i != static_cast<std::size_t>(-1); --i) {
          fixDown(i);
        }
      }
    }

    void fixDown(std::size_t index)
      noexcept(std::is_nothrow_swappable_v<T>) {
      using std::swap;
      for (;;) {
        std::size_t child = getLeftChild(index);
        if (child >= data_.size()) {
          break;
        }

        // Choose the smaller one between the two child nodes.
        std::size_t right = child + 1;
        if (right < data_.size() && comparator_(data_[right], data_[child])) {
          child = right;
        }

        if (comparator_(data_[child], data_[index])) {
          swap(data_[child], data_[index]);
          index = child;
        } else {
          break;
        }
      }
    }

    void fixUp(std::size_t index) 
      noexcept(std::is_nothrow_swappable_v<T>) {
      using std::swap;

      while (hasParent(index)) {
        std::size_t parent = getParent(index);
        if (comparator_(data_[index], data_[parent])) {
          swap(data_[index], data_[parent]);
          index = parent;
        } else {
          break;
        }
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
