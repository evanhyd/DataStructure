#pragma once
#include <iterator>
#include <memory>
#include <utility>

namespace flow::pmr {

  /// <summary>
  /// Constructs objects in uninitialized memory by copying from a source range.
  /// </summary>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedCopy(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) {
    for (; first != last; ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// <summary>
  /// Constructs objects in uninitialized memory by moving from a source range.
  /// </summary>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMove(AllocatorType& allocator, InputIt first, InputIt last, OutputIt dest) noexcept {
    for (; first != last; ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), std::move(*first));
    }
    return dest;
  }

  /// <summary>
  /// Fills uninitialized memory with copies of a specified value.
  /// </summary>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFill(AllocatorType& allocator, OutputIt first, OutputIt last, const T& value) {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), value);
    }
    return first;
  }

  /// <summary>
  /// Destroys a range of constructed objects.
  /// </summary>
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElements(AllocatorType& allocator, InputIt first, InputIt last) noexcept {
    for (; first != last; ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// <summary>
  /// Constructs a fixed number of objects by copying from an input iterator.
  /// </summary>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedCopyN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) {
    for (std::size_t i = 0; i < count; ++i, ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), *first);
    }
    return dest;
  }

  /// <summary>
  /// Constructs a fixed number of objects by moving from an input iterator.
  /// </summary>
  template <typename AllocatorType, std::input_iterator InputIt, std::forward_iterator OutputIt>
  OutputIt uninitializedMoveN(AllocatorType& allocator, InputIt first, std::size_t count, OutputIt dest) noexcept {
    for (std::size_t i = 0; i < count; ++i, ++first, ++dest) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*dest), std::move(*first));
    }
    return dest;
  }

  /// <summary>
  /// Fills a fixed number of uninitialized elements with a given value.
  /// </summary>
  template <typename AllocatorType, std::forward_iterator OutputIt, typename T>
  OutputIt uninitializedFillN(AllocatorType& allocator, OutputIt first, std::size_t count, const T& value) {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::construct(allocator, std::addressof(*first), value);
    }
    return first;
  }

  /// <summary>
  /// Destroys a fixed number of constructed elements.
  /// </summary>
  template <typename AllocatorType, std::forward_iterator InputIt>
  void destroyElementsN(AllocatorType& allocator, InputIt first, std::size_t count) noexcept {
    for (std::size_t i = 0; i < count; ++i, ++first) {
      std::allocator_traits<AllocatorType>::destroy(allocator, std::addressof(*first));
    }
  }

  /// <summary>
  /// Destroys and deallocates the buffer. 
  /// The size must equal to the number of constructed element.
  /// The capacity must equal to the buffer capacity.
  /// </summary>
  template <typename AllocatorType, typename T>
  void deleteBuffer(AllocatorType& allocator, T* buffer, std::size_t size, std::size_t capacity) noexcept {
    destroyElementsN(allocator, buffer, size);
    std::allocator_traits<AllocatorType>::deallocate(allocator, buffer, capacity);
  }
}
