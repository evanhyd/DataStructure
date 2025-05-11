#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "basic_allocator.h"
#include "tuple.h"

namespace flow {
  template <typename T, template<typename> typename Allocator = BasicAllocator>
  class Vector final {
  public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using allocator_type = Allocator<T>;

  private:
    allocator_type allocator_{};
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    T* buf_{};

    // Calculate the new capacity from the given capacity.
    static constexpr std::size_t next_capacity(std::size_t currentCapacity) {
      return currentCapacity * 2 + 1;
    }

    // Copy construct a range of elements at the output destination.
    template <std::input_iterator It>
    iterator copy_uninit(It first, It last, iterator out) {
      for (; first != last; ++out, ++first) {
        allocator_.construct(out, *first);
      }
      return out;
    }

    // Move construct a range of elements at the output destination.
    template <std::input_iterator It>
    iterator move_uninit(It first, It last, iterator out) noexcept {
      for (; first != last; ++out, ++first) {
        allocator_.construct(out, std::move(*first));
      }
      return out;
    }

    // Copy value and construct the elements at the output destination.
    void fill_uninit(iterator first, iterator last, const T& value) {
      for (; first != last; ++first) {
        allocator_.construct(first, value);
      }
    }

    // Destroy the elements at the given range.
    void destroy(iterator first, iterator last) noexcept {
      for (; first != last; ++first) {
        allocator_.destroy(first);
      }
    }

    // Destroy and deallocate all the elements.
    void delete_all() noexcept {
      destroy(begin(), end());
      allocator_.deallocate(buf_, capacity_);
    }

    // Relocate the array.
    void relocate(std::size_t capacity) {
      T* buf = allocator_.allocate(capacity);
      move_uninit(begin(), end(), buf);

      delete_all();
      buf_ = buf;
      capacity_ = capacity;
    }

    // Relocate the array, then right shift the elements to reserve buffer in the middle.
    void relocate_with_hole(std::size_t capacity, iterator pos, std::size_t holeSize) {
      T* buf = allocator_.allocate(capacity);
      move_uninit(pos, end(), move_uninit(begin(), pos, buf) + holeSize);

      delete_all();
      buf_ = buf;
      capacity_ = capacity;
    }

    // Relocate the array, and insert the new elements then move the old elements.
    // New elements are copied first because the range may refer to the elements from the old buffer.
    template <std::forward_iterator it>
    void relocate_with_insert(std::size_t capacity, iterator pos, it first, it last) {
      T* buf = allocator_.allocate(capacity);
      iterator where = buf + (pos - buf_); // Inserting position in the new buffer.
      move_uninit(pos, end(), copy_uninit(first, last, where)); // Copy the range and move the second half.
      move_uninit(begin(), pos, buf); // Move the first half.

      delete_all();
      buf_ = buf;
      capacity_ = capacity;
    }

  public:
    explicit constexpr Vector(const allocator_type& allocator = allocator_type())
      : allocator_(allocator) {
    }

    constexpr Vector(const Vector<T>& rhs)
      : Vector(rhs.begin(), rhs.end(), rhs.allocator_) {
    }

    constexpr Vector(Vector<T>&& rhs) noexcept
      : allocator_(std::exchange(rhs.allocator_, allocator_type())),
        size_(std::exchange(rhs.size_, 0)), 
        capacity_(std::exchange(rhs.capacity_, 0)),
        buf_(std::exchange(rhs.buf_, nullptr)) {
    }

    constexpr Vector(std::initializer_list<T> list, const allocator_type& allocator = allocator_type())
      : Vector(list.begin(), list.end(), allocator) {
    }

    template <std::input_iterator It>
    explicit constexpr Vector(It first, It last, const allocator_type& allocator = allocator_type())
      : allocator_(allocator),
        size_(std::distance(first, last)), 
        capacity_(size_),
        buf_(allocator_.allocate(size_)) {
      copy_uninit(first, last, begin());
    }

    explicit constexpr Vector(std::size_t count, const T& value = T{}, const allocator_type& allocator = allocator_type())
      : allocator_(allocator),
        size_(count),
        capacity_(count),
        buf_(allocator_.allocate(count)) {
      fill_uninit(begin(), end(), value);
    }

    ~Vector() {
      delete_all();
    }

    Vector& operator=(Vector rhs) noexcept {
      swap(*this, rhs);
      return *this;
    }

    T& operator[](std::size_t i) {
      assert(i < size_ && "index out of bound");
      return buf_[i];
    }

    const T& operator[](std::size_t i) const {
      assert(i < size_ && "index out of bound");
      return buf_[i];
    }

    constexpr std::size_t size() const {
      return size_;
    }

    constexpr std::size_t capacity() const {
      return capacity_;
    }

    constexpr bool empty() const {
      return size_ == 0;
    }

    void clear() noexcept {
      destroy(begin(), end());
      size_ = 0;
    }

    T& front() {
      assert(size_ != 0 && "access empty Vector front");
      return buf_[0];
    }

    constexpr const T& front() const {
      assert(size_ != 0 && "access empty Vector front");
      return buf_[0];
    }

    T& back() {
      assert(size_ != 0 && "access empty Vector back");
      return buf_[size_ - 1];
    }

    constexpr const T& back() const {
      assert(size_ != 0 && "access empty Vector back");
      return buf_[size_ - 1];
    }

    iterator begin() {
      return buf_;
    }

    constexpr const_iterator begin() const {
      return buf_;
    }

    iterator end() {
      return buf_ + size_;
    }

    constexpr const_iterator end() const {
      return buf_ + size_;
    }

    void reserve(std::size_t capacity) {
      if (capacity_ < capacity) {
        relocate(capacity);
      }
    }

    void resize(std::size_t size, T value = T{}) {
      if (size_ < size) {
        //expand
        if (capacity_ < size) {
          relocate(size);
        }
        fill_uninit(begin() + size_, begin() + size, value);
      } else {
        //shrink
        destroy(begin() + size, begin() + size_);
      }
      size_ = size;
    }

    template <typename ...Args>
    void push_back(Args&&... args) {
      if (size_ == capacity_) {
        //https://stackoverflow.com/questions/5232198/how-does-the-capacity-of-stdvector-grow-automatically-what-is-the-rate
        relocate(next_capacity(capacity_));
      }
      allocator_.construct(buf_ + size_, std::forward<Args>(args)...);
      ++size_;
    }

    void pop_back() noexcept {
      assert(size_ > 0 && "can not pop back from an empty vector");
      --size_;
      allocator_.destroy(end());
    }

    void erase(iterator first, iterator last) noexcept {
      //replace the erased elements then destroy the rest
      destroy(std::move(last, end(), first), end());
      size_ -= last - first;
    }

    void erase(iterator pos) noexcept {
      erase(pos, pos + 1);
    }

    template <typename ...Args>
    void insert(iterator pos, Args&&... value) {
      // Special case: append at the end.
      if (pos == end()) {
        push_back(std::forward<Args>(value)...);
        return;
      }

      if (size_ < capacity_) {
        // Enough capacity, right shift the elements.
        // Major optimization to use memcpy, copy_backward, or range move_backward instead of handroll loop. A 70% reduction in computation time.
        allocator_.construct(end(), std::move(*(end()-1)));
        std::move_backward(pos, end() - 1, end());
        *pos = T(value...);

      } else {
        // Not enough capacity, relocate everything.
        std::size_t index = pos - buf_;
        relocate_with_hole(next_capacity(capacity_), pos, 1);
        allocator_.construct(buf_ + index, std::forward<Args>(value)...);
      }
      ++size_;
    }

    template <std::input_iterator It>
    void insert(iterator pos, It first, It last) {
      std::size_t requiredSize = size_ + std::distance(first, last);
      if (pos == end() && requiredSize <= capacity_) {
        // Special case: append at the end.
        // Reallocation must not occur, otherwise it may invalidate the input iterators if self-inserting.
        copy_uninit(first, last, end());
      } else {
        // Must relocate regardless the inserting position.
        // TODO: Optimize opportunity: inserting middle but with enough capacity.
        relocate_with_insert(requiredSize, pos, first, last);
      }
      size_ = requiredSize;
    }

    template<typename MappingFn, template<typename> typename Alloc = Allocator>
    Vector<std::invoke_result_t<MappingFn, const T&>, Alloc> map(const MappingFn& fn) const {
      Vector<std::invoke_result_t<MappingFn, const T&>, Alloc> mapped;
      mapped.relocate(size_);
      for (const T& val : *this) {
        mapped.push_back(fn(val));
      }
      return mapped;
    }

    template <typename FilterFn, template<typename> typename Alloc = Allocator>
    Vector<T, Alloc> filter(const FilterFn& fn) const {
      static_assert(std::is_same_v<std::invoke_result_t<FilterFn, const T&>, bool>, "filter function must evaluate to bool");
      Vector<T, Alloc> filtered;
      for (const T& val : *this) {
        if (fn(val)) {
          filtered.push_back(val);
        }
      }
      return filtered;
    }

    template <typename CallbackFn>
    void for_each(const CallbackFn& fn) const {
      for (auto& val : *this) {
        fn(val);
      }
    }

    friend void swap(Vector& lhs, Vector& rhs) {
      using std::swap;
      swap(lhs.allocator_, rhs.allocator_);
      swap(lhs.buf_, rhs.buf_);
      swap(lhs.size_, rhs.size_);
      swap(lhs.capacity_, rhs.capacity_);
    }

    friend class Vector;
  };

  template <typename T, template<typename> typename A, template<typename> typename B>
  bool operator==(const flow::Vector<T, A>& lhs, const flow::Vector<T, B>& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (size_t i = 0, j = lhs.size(); i < j; ++i) {
      if (lhs[i] != rhs[i]) {
        return false;
      }
    }
    return true;
  }

  template <typename T, template<typename> typename A, template<typename> typename B>
  bool operator!=(const flow::Vector<T, A>& lhs, const flow::Vector<T, B>& rhs) {
    return !(lhs == rhs);
  }

  //TODO: support move semantics
  template <typename ...Vec>
  Vector<Tuple<typename Vec::value_type...>> zip(const Vec&... vec) {
    using ZipType = Tuple<typename Vec::value_type...>;
    std::size_t minSize = std::min({vec.size()...});
    Vector<ZipType> zipped;
    zipped.reserve(minSize);
    for (std::size_t i = 0; i < minSize; ++i) {
      zipped.push_back(ZipType{vec[i]...});
    }
    return zipped;
  }

  Vector<std::string> split(const std::string& line, const std::string& delimiter) {
    if (delimiter.empty()) {
      return { line };
    }

    Vector<std::string> tokens;
    for (std::size_t pos = 0;;) {
      std::size_t nextPos = line.find(delimiter, pos);
      tokens.push_back(line.substr(pos, nextPos - pos));
      if (nextPos == std::string::npos) {
        break;
      }
      pos = nextPos + delimiter.size();
    }
    return tokens;
  }

  std::string join(const Vector<std::string>& tokens, const std::string& separator) {
    if (tokens.empty()) {
      return "";
    }

    std::size_t sz = (tokens.size() - 1) * separator.size();
    for (auto& token : tokens) {
      sz += token.size();
    }

    std::string line;
    line.reserve(sz);
    for (std::size_t i = 0; i + 1 < tokens.size(); ++i) {
      line += tokens[i] + separator;
    }
    line += tokens.back();
    return line;
  }
}
