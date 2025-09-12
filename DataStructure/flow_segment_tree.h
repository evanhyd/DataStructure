#pragma once
#include "flow_vector.h"

namespace flow {

  template <typename T, typename BinOp, typename Allocator = PolymorphicAllocator<>>
  class SegmentTree {
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
    BinOp binOp_;

  public:
    constexpr SegmentTree(const SegmentTree&) = default;
    constexpr SegmentTree(const SegmentTree& rhs, const allocator_type& allocator)
      : data_(rhs.data_, allocator), binOp_(rhs.binOp_) {
    }

    constexpr SegmentTree(SegmentTree&&) = default;
    constexpr SegmentTree(SegmentTree&& rhs, const allocator_type& allocator)
      noexcept(noexcept(container_type(std::move(rhs.data_), allocator)) && noexcept(BinOp(std::move(rhs.binOp_))))
      : data_(std::move(rhs.data_), allocator), binOp_(std::move(rhs.binOp_)) {
    }

    /// @brief Iterator range constructor.
    /// @param first Iterator to first element.
    /// @param last Iterator to one-past-last element.
    /// @param binOp Binary operator.
    /// @param alloc Allocator.
    template <std::input_iterator It>
    explicit constexpr SegmentTree(It first, It last, BinOp binOp, const allocator_type& allocator = {})
      : data_(first, last, allocator), binOp_(std::move(binOp)) {

    }

    /// @brief Initializer list constructor.
    /// @param list Initializer list.
    /// @param binOp Binary operator.
    /// @param alloc Allocator.
    explicit constexpr SegmentTree(std::initializer_list<T> list, BinOp binOp, const allocator_type& allocator = {})
      : data_(list, allocator), binOp_(std::move(binOp)) {
    }

    ~SegmentTree() = default;

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return data_.get_allocator();
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
