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
    explicit constexpr SegmentTree(It first, It last, BinOp binOp = {}, const allocator_type& allocator = {})
      : data_(std::distance(first, last) * 2, allocator), binOp_(std::move(binOp)) {
      std::copy(first, last, data_.begin() + size());
      buildParent();
    }

    /// @brief Initializer list constructor.
    /// @param list Initializer list.
    /// @param binOp Binary operator.
    /// @param alloc Allocator.
    explicit constexpr SegmentTree(std::initializer_list<T> list, BinOp binOp = {}, const allocator_type& allocator = {})
      : data_(list.size() * 2, allocator), binOp_(std::move(binOp)) {
      std::copy(list.begin(), list.end(), data_.begin() + size());
      buildParent();
    }

    ~SegmentTree() = default;

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return data_.get_allocator();
    }

    // Accessor.

    /// @brief Return the number of input elements. This only counts the leaf node from the input.
    /// @return The number of elements.
    std::size_t size() const {
      return data_.size() / 2;
    }

    /// @brief Set the new value at index i, then update the tree structure.
    /// @param i The index.
    /// @param value New value.
    void setPoint(std::size_t i, const T& value) {
      i += size();
      data_[i] = value;
      while (hasParent(i)) {
        std::size_t parent = getParent(i);
        std::size_t sibling = getSibling(i);
        data_[parent] = binOp_(data_[i], data_[sibling]);
        i = parent;
      }
    }

    /// @brief Query the value from [first, last) after applying the binary operator adjacent-pair wise.
    /// The range must not be empty, otherwise the result is undefined.
    /// @param first The begin index.
    /// @param last The end index.
    /// @return The range value.
    T getRange(std::size_t first, std::size_t last) const {
      assert(first < last && "first must be smaller than last");
      std::size_t elementSize = size();
      first += elementSize;
      last += elementSize;

      std::optional<T> result{};

      for (; first < last;) {
        if (isRightChild(first)) {
          if (!result) {
            result = data_[first];
          } else {
            *result = binOp_(*result, data_[first]);
          }
          ++first;
        }

        if (isRightChild(last)) {
          --last;
          if (!result) {
            result = data_[last];
          } else {
            *result = binOp_(*result, data_[last]);
          }
        }

        first = getParent(first);
        last = getParent(last);
      }

      return *result;
    }

  private:
    void buildParent() {
      for (std::size_t i = size(); i-- > 1;) {
        std::size_t left = getLeftChild(i);
        std::size_t right = left + 1;
        data_[i] = binOp_(data_[left], data_[right]);
      }
    }

    static constexpr bool hasParent(std::size_t i) noexcept {
      return i > 1;
    }

    static constexpr std::size_t getParent(std::size_t i) noexcept {
      assert(hasParent(i) && "calling parent on the root node");
      return i / 2;
    }

    static constexpr std::size_t getLeftChild(std::size_t i) noexcept {
      return i * 2;
    }

    static constexpr std::size_t getSibling(std::size_t i) noexcept {
      return i ^ 1;
    }

    static constexpr bool isLeftChild(std::size_t i) noexcept {
      return (i % 2) == 0;
    }

    static constexpr bool isRightChild(std::size_t i) noexcept {
      return !isLeftChild(i);
    }
  };
}
