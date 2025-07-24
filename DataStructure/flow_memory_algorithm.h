#pragma once
#include <cassert>
#include <iterator>
#include <memory>

namespace flow {

  /// @brief Forward elements from a source range to uninitialized memory.
  /// @param allocator Allocator for construction.
  /// @param first Start of source range.
  /// @param last End of source range.
  /// @param dest Start of destination range.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedForward(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) {
    for (; first != last; ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// @brief Moves elements from a source range to uninitialized memory.
  /// @param allocator Allocator for construction.
  /// @param first Start of source range.
  /// @param last End of source range.
  /// @param dest Start of destination range.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMove(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) noexcept {
    return uninitializedForward(allocator, std::move_iterator(first), std::move_iterator(last), dest);
  }

  /// @brief Constructs objects in uninitialized memory by copying arguments to their constructor.
  ///        Intentional copy of arguments to prevent reuse of moved objects.
  /// @param allocator Allocator for memory management.
  /// @param first Start of the destination range.
  /// @param last End of the destination range.
  /// @param args Constructor arguments.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::forward_iterator OutputIt, typename ...Args>
  OutputIt uninitializedEmplace(AllocatorType& allocator, OutputIt first, OutputIt last, const Args&... args) {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), args...);
    }
    return first;
  }

  /// @brief Fills uninitialized memory with copies of a value.
  /// @param allocator Allocator for construction.
  /// @param first Start of destination range.
  /// @param last End of destination range.
  /// @param value Value to fill with.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFill(AllocatorType& allocator, OutputIt first, OutputIt last, const T& value) {
    return uninitializedEmplace(allocator, first, last, value);
  }

  /// @brief Destroys a range of constructed objects in memory.
  /// @param allocator Allocator for destruction.
  /// @param first Start of range.
  /// @param last End of range.
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElements(AllocatorType& allocator, InputIt first, InputIt last) noexcept {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// @brief Forward `count` elements from a source range to uninitialized memory.
  /// @param allocator Allocator for construction.
  /// @param first Start of source range.
  /// @param count Number of elements to copy.
  /// @param dest Start of destination range.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedForwardN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) {
    for (std::size_t i = 0; i < count; ++i, ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// @brief Moves `count` elements from a source range to uninitialized memory.
  /// @param allocator Allocator for construction.
  /// @param first Start of source range.
  /// @param count Number of elements to move.
  /// @param dest Start of destination range.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMoveN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) noexcept {
    return uninitializedForwardN(allocator, std::move_iterator(first), count, dest);
  }

  /// @brief Constructs a specified number of objects in uninitialized memory by copying constructor arguments.
  ///        Intentional copy of arguments to prevent reuse of moved objects.
  /// @param allocator Allocator for memory management.
  /// @param first Start of the destination range.
  /// @param count Number of objects to construct.
  /// @param args Constructor arguments.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::forward_iterator OutputIt, typename ...Args>
  OutputIt uninitializedEmplaceN(AllocatorType& allocator, OutputIt first, std::size_t count, const Args&... args) {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), args...);
    }
    return first;
  }

  /// @brief Fills `count` elements in uninitialized memory with a value.
  /// @param allocator Allocator for construction.
  /// @param first Start of destination range.
  /// @param count Number of elements to fill.
  /// @param value Value to fill with.
  /// @return One past the last constructed element.
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFillN(AllocatorType& allocator, OutputIt first, std::size_t count, const T& value) {
    return uninitializedEmplaceN(allocator, first, count, value);
  }

  /// @brief Destroys `count` objects in a range.
  /// @param allocator Allocator for destruction.
  /// @param first Start of range.
  /// @param count Number of elements to destroy.
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElementsN(AllocatorType& allocator, InputIt first, std::size_t count) noexcept {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// @brief Destroys and deallocates the buffer.
  /// @param allocator Allocator for destruction and deallocation.
  /// @param buffer Pointer to the buffer.
  /// @param size Number of constructed elements.
  /// @param capacity Total buffer capacity.
  template <typename AllocatorType, typename T>
  void deleteBuffer(AllocatorType& allocator, T* buffer, std::size_t size, std::size_t capacity) noexcept {
    destroyElementsN(allocator, buffer, size);
    std::allocator_traits<AllocatorType>::deallocate(allocator, buffer, capacity);
  }

  /// @brief Calculate the distance in bytes from the first pointer to the last pointer.
  /// @param first Pointer to the first element.
  /// @param last Pointer to one-past-last element.
  /// @return The address distance in bytes.
  template <typename T, typename U>
  std::size_t pointerDistance(const T* first, const U* last) {
    assert(first <= last && "first pointer address must be smaller than the last pointer address");
    return reinterpret_cast<const std::byte*>(last) - reinterpret_cast<const std::byte*>(first);
  }

  /// @brief Align the header + buffer to their corresponding alignments.
  ///        If the capacity is not big enough, returns nullptr.
  /// @param alignment Required alignment.
  /// @param size Size of the buffer to align.
  /// @param buffer Reference to pointer to the buffer to be aligned.
  /// @param capacity Reference to the capacity of the buffer.
  /// @return Aligned header pointer followed by the aligned buffer, or nullptr on failure.
  template <typename Header>
  Header* alignWithHeader(std::size_t alignment, std::size_t size, void*& buffer, std::size_t& capacity) noexcept {

    // https://stackoverflow.com/questions/46457449/is-it-always-the-case-that-sizeoft-alignoft-for-all-object-types-t
    assert(size >= alignment && "size is smaller than its alignment");
    if (capacity < sizeof(Header) + size) {
      return nullptr;
    }

    // Set the block alignment to be at least as big as the header alignment.
    alignment = std::max(alignment, alignof(Header));

    // Reserve at least sizeof(Header) before the allocated block.
    void* allocatedBlock = reinterpret_cast<Header*>(buffer) + 1;
    std::size_t capacityAfterHeader = capacity - sizeof(Header);
    if (!std::align(alignment, size, allocatedBlock, capacityAfterHeader)) {
      return nullptr;
    }

    // Calculate the address of the header.
    Header* header = reinterpret_cast<Header*>(allocatedBlock) - 1;
    assert(reinterpret_cast<std::uintptr_t>(header) % alignof(Header) == 0 && "allocated buffer is not aligned with the header");

    capacity -= pointerDistance(buffer, header);
    buffer = header;
    return header;
  }

}
