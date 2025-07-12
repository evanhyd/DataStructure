#pragma once
#include <iterator>
#include <memory>
#include <utility>

namespace flow {

  /// <summary>
  /// Forward elements from a source range to uninitialized memory.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Input iterator for source range.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of source range.</param>
  /// <param name="last">End of source range.</param>
  /// <param name="dest">Start of destination range.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedForward(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) {
    for (; first != last; ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// <summary>
  /// Moves elements from a source range to uninitialized memory.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Input iterator for source range.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of source range.</param>
  /// <param name="last">End of source range.</param>
  /// <param name="dest">Start of destination range.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMove(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) noexcept {
    return uninitializedForward(allocator, std::move_iterator(first), std::move_iterator(last), dest);
  }

  /// <summary>
  /// Constructs objects in uninitialized memory by copying arguments to their constructor.
  /// Is is intentional that the arguments are copied instead of forwarded to prevent reuse moved objects.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type for memory management.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for the destination range.</typeparam>
  /// <typeparam name="Args">Types of arguments copied to the constructor.</typeparam>
  /// <param name="allocator">Allocator for memory management.</param>
  /// <param name="first">Start of the destination range.</param>
  /// <param name="last">End of the destination range.</param>
  /// <param name="args">Constructor arguments.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename ...Args>
  OutputIt uninitializedEmplace(AllocatorType& allocator, OutputIt first, OutputIt last, const Args&... args) {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), args...);
    }
    return first;
  }

  /// <summary>
  /// Fills uninitialized memory with copies of a value.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <typeparam name="T">Type of value.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of destination range.</param>
  /// <param name="last">End of destination range.</param>
  /// <param name="value">Value to fill with.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFill(AllocatorType& allocator, OutputIt first, OutputIt last, const T& value) {
    return uninitializedEmplace(allocator, first, last, value);
  }

  /// <summary>
  /// Destroys a range of constructed objects in memory.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Forward iterator for range.</typeparam>
  /// <param name="allocator">Allocator for destruction.</param>
  /// <param name="first">Start of range.</param>
  /// <param name="last">End of range.</param>
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElements(AllocatorType& allocator, InputIt first, InputIt last) noexcept {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// <summary>
  /// Forward `count` elements from a source range to uninitialized memory.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Input iterator for source range.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of source range.</param>
  /// <param name="count">Number of elements to copy.</param>
  /// <param name="dest">Start of destination range.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedForwardN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) {
    for (std::size_t i = 0; i < count; ++i, ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// <summary>
  /// Moves `count` elements from a source range to uninitialized memory.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Input iterator for source range.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of source range.</param>
  /// <param name="count">Number of elements to move.</param>
  /// <param name="dest">Start of destination range.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMoveN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) noexcept {
    return uninitializedForwardN(allocator, std::move_iterator(first), count, dest);
  }

  /// <summary>
  /// Constructs a specified number of objects in uninitialized memory by forwarding arguments to their constructor.
  /// Is is intentional that the arguments are copied instead of forwarded to prevent reuse moved objects.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type for memory management.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for the destination range.</typeparam>
  /// <typeparam name="Args">Types of arguments copied to the constructor.</typeparam>
  /// <param name="allocator">Allocator for memory management.</param>
  /// <param name="first">Start of the destination range.</param>
  /// <param name="count">Number of objects to construct.</param>
  /// <param name="args">Constructor arguments.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename ...Args>
  OutputIt uninitializedEmplaceN(AllocatorType& allocator, OutputIt first, std::size_t count, const Args&... args) {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), args...);
    }
    return first;
  }

  /// <summary>
  /// Fills `count` elements in uninitialized memory with a value.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="OutputIt">Forward iterator for destination range.</typeparam>
  /// <typeparam name="T">Type of value.</typeparam>
  /// <param name="allocator">Allocator for construction.</param>
  /// <param name="first">Start of destination range.</param>
  /// <param name="count">Number of elements to fill.</param>
  /// <param name="value">Value to fill with.</param>
  /// <returns>One past the last constructed element.</returns>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFillN(AllocatorType& allocator, OutputIt first, std::size_t count, const T& value) {
    return uninitializedEmplaceN(allocator, first, count, value);
  }

  /// <summary>
  /// Destroys `count` objects in a range.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="InputIt">Forward iterator for range.</typeparam>
  /// <param name="allocator">Allocator for destruction.</param>
  /// <param name="first">Start of range.</param>
  /// <param name="count">Number of elements to destroy.</param>
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElementsN(AllocatorType& allocator, InputIt first, std::size_t count) noexcept {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// <summary>
  /// Destroys and deallocates the buffer, where `size` is the number of constructed elements
  /// and `capacity` is the buffer capacity.
  /// </summary>
  /// <typeparam name="AllocatorType">Allocator type.</typeparam>
  /// <typeparam name="T">Type of buffer elements.</typeparam>
  /// <param name="allocator">Allocator for destruction and deallocation.</param>
  /// <param name="buffer">Pointer to the buffer.</param>
  /// <param name="size">Number of constructed elements.</param>
  /// <param name="capacity">Total buffer capacity.</param>
  template <typename AllocatorType, typename T>
  void deleteBuffer(AllocatorType& allocator, T* buffer, std::size_t size, std::size_t capacity) noexcept {
    destroyElementsN(allocator, buffer, size);
    std::allocator_traits<AllocatorType>::deallocate(allocator, buffer, capacity);
  }
}
