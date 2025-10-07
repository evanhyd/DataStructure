#pragma once
#include "flow_vector.h"

namespace flow {

  /// @brief A disjoint union set that uses path compression and union by rank.
  /// @tparam T 
  /// @tparam Allocator 
  template <typename T, typename Allocator = PolymorphicAllocator<T>>
  class DisjointSet {
    struct Node {
      std::size_t groupId;
      std::size_t rank;
      T value;
    };

  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using difference_type = std::ptrdiff_t;
    using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using container_type = Vector<Node, allocator_type>;

  private:
    std::size_t groupSize_;
    container_type data_;

  public:
    constexpr DisjointSet()
      noexcept(noexcept(container_type()))
      : groupSize_(0), data_() {
    }

    explicit constexpr DisjointSet(const allocator_type& allocator)
      noexcept(noexcept(container_type(allocator)))
      : groupSize_(0), data_(allocator) {
    }

    constexpr DisjointSet(const DisjointSet&) = default;
    constexpr DisjointSet(const DisjointSet& rhs, const allocator_type& allocator)
      : groupSize_(rhs.groupSize_), data_(rhs.data_, allocator) {
    }

    constexpr DisjointSet(DisjointSet&&) = default;
    constexpr DisjointSet(DisjointSet&& rhs, const allocator_type& allocator)
      noexcept(noexcept(container_type(std::move(rhs.data_), allocator)))
      : groupSize_(rhs.groupSize_), data_(std::move(rhs.data_), allocator) {
      // rhs groupSize may not match up with the actual group size (0).
      // That's fine, because we won't use that value after the move.
      // Hopefully it won't come back and bite me in the future.
      // Put a cat girl ascii here for good luck:
      /*
      ⣿⡟⠙⠛⠋⠩⠭⣉⡛⢛⠫⠭⠄⠒⠄⠄⠄⠈⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿
      ⣿⡇⠄⠄⠄⠄⣠⠖⠋⣀⡤⠄⠒⠄⠄⠄⠄⠄⠄⠄⠄⠄⣈⡭⠭⠄⠄⠄⠉⠙
      ⣿⡇⠄⠄⢀⣞⣡⠴⠚⠁⠄⠄⢀⠠⠄⠄⠄⠄⠄⠄⠄⠉⠄⠄⠄⠄⠄⠄⠄⠄
      ⣿⡇⠄⡴⠁⡜⣵⢗⢀⠄⢠⡔⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄
      ⣿⡇⡜⠄⡜⠄⠄⠄⠉⣠⠋⠠⠄⢀⡄⠄⠄⣠⣆⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⢸
      ⣿⠸⠄⡼⠄⠄⠄⠄⢰⠁⠄⠄⠄⠈⣀⣠⣬⣭⣛⠄⠁⠄⡄⠄⠄⠄⠄⠄⢀⣿
      ⣏⠄⢀⠁⠄⠄⠄⠄⠇⢀⣠⣴⣶⣿⣿⣿⣿⣿⣿⡇⠄⠄⡇⠄⠄⠄⠄⢀⣾⣿
      ⣿⣸⠈⠄⠄⠰⠾⠴⢾⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢁⣾⢀⠁⠄⠄⠄⢠⢸⣿⣿
      ⣿⣿⣆⠄⠆⠄⣦⣶⣦⣌⣿⣿⣿⣿⣷⣋⣀⣈⠙⠛⡛⠌⠄⠄⠄⠄⢸⢸⣿⣿
      ⣿⣿⣿⠄⠄⠄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠈⠄⠄⠄⠄⠄⠈⢸⣿⣿
      ⣿⣿⣿⠄⠄⠄⠘⣿⣿⣿⡆⢀⣈⣉⢉⣿⣿⣯⣄⡄⠄⠄⠄⠄⠄⠄⠄⠈⣿⣿
      ⣿⣿⡟⡜⠄⠄⠄⠄⠙⠿⣿⣧⣽⣍⣾⣿⠿⠛⠁⠄⠄⠄⠄⠄⠄⠄⠄⠃⢿⣿
      ⣿⡿⠰⠄⠄⠄⠄⠄⠄⠄⠄⠈⠉⠩⠔⠒⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠐⠘⣿
      ⣿⠃⠃⠄⠄⠄⠄⠄⠄⣀⢀⠄⠄⡀⡀⢀⣤⣴⣤⣤⣀⣀⠄⠄⠄⠄⠄⠄⠁⢹
      */
    }

    /// @brief Iterator range constructor.
    /// @param first Iterator to first element.
    /// @param last Iterator to one-past-last element.
    /// @param alloc Allocator.
    template <std::forward_iterator It>
    explicit constexpr DisjointSet(It first, It last, const allocator_type& allocator = {})
      : groupSize_(std::distance(first, last)), data_(allocator) {
      data_.reserve(groupSize_);
      for (std::size_t i = 0; i < groupSize_; ++i) {
        data_.pushBack(Node{ i, 0, *first });
        ++first;
      }
    }

    /// @brief Initializer list constructor.
    /// @param list Initializer list.
    /// @param alloc Allocator.
    explicit constexpr DisjointSet(std::initializer_list<T> list, const allocator_type& allocator = {})
      : DisjointSet(list.begin(), list.end(), allocator) {
    }

    ~DisjointSet() = default;

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return data_.get_allocator();
    }

    // Accessor.

    /// @brief Return the number of input elements.
    /// @return The number of elements.
    constexpr std::size_t size() const {
      return data_.size();
    }

    /// @brief Return the number of group.
    /// @return The number of group.
    constexpr std::size_t groupSize() const {
      return groupSize_;
    }

    /// @brief Check if the disjoint set is empty.
    /// @return true if the disjoint set is empty.
    constexpr bool empty() const {
      return data_.empty();
    }

    /// @brief Reserve enough memory for at least n elements.
    /// @param n The reserved size.
    void reserve(std::size_t n) {
      data_.reserve(n);
    }

    /// @brief Add an element to the disjoint set.
    /// @param value The element.
    constexpr void add(const T& value) {
      data_.pushBack(Node{ data_.size(), 0, value });
      ++groupSize_;
    }

    /// @brief Add an element to the disjoint set.
    /// @param value The element.
    constexpr void add(T&& value) {
      data_.pushBack(Node{ data_.size(), 0, std::move(value) });
      ++groupSize_;
    }

    /// @brief Get the value of the i-th element.
    /// @param i index 
    /// @return The value of the i-th element.
    constexpr const T& operator[](std::size_t i) const {
      return data_[i].value;
    }

    /// @brief Get the value of the i-th element.
    /// @param i index 
    /// @return The value of the i-th element.
    constexpr T& operator[](std::size_t i) {
      return data_[i].value;
    }

    /// @brief Find the group id of the i-th node.
    /// Perform path compression along the way.
    /// @param i Index
    /// @return The group id of the i-th node.
    constexpr std::size_t find(std::size_t i) {
      // Can switch to two passes using loop if overflows.
      if (data_[i].groupId == i) {
        return i;
      }
      return data_[i].groupId = find(data_[i].groupId);
    }

    /// @brief Check if the i-th element and the j-th element are in the same group.
    /// @param i The first index.
    /// @param j The second index.
    /// @return True if in the same group.
    constexpr bool isSameGroup(std::size_t i, std::size_t j) {
      return find(i) == find(j);
    }

    /// @brief Merge the i-th element's group with the j-th element's group.
    /// Internally, it unions them by ranking.
    /// @param i The first index.
    /// @param j The second index.
    /// @return True if merge successfully.
    constexpr bool merge(std::size_t i, std::size_t j) {
      std::size_t p1 = find(i);
      std::size_t p2 = find(j);
      if (p1 == p2) {
        return false;
      }

      if (data_[p1].rank <= data_[p2].rank) {
        data_[p1].groupId = p2;
        if (data_[p1].rank == data_[p2].rank) {
          ++data_[p2].rank;
        }
      } else {
        data_[p2].groupId = p1;
      }

      --groupSize_;
      return true;
    }
  };
}
