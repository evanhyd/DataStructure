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

#include "allocator.h"
#include "tuple.h"

namespace flow {
  template <typename T, template<typename> typename Allocator = BasicAllocator>
  class Vector final {
  public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

  private:
    Allocator<T> allocator_{};
    std::size_t size_{};
    std::size_t capacity_{};
    T* buf_{};

    [[maybe_unused]] void range_check([[maybe_unused]] iterator first, [[maybe_unused]] iterator last) {
      assert(buf_ <= first && first <= buf_ + capacity_ && "first interator out of bound");
      assert(buf_ <= last && last <= buf_ + capacity_ && "last iterator out of bound");
      assert(first <= last && "first iterator denotes a position after last iterator");
    }

    template <std::input_iterator It>
    iterator copy_uninit(It first, It last, iterator output) {
      for (; first != last; ++output, ++first) {
        allocator_.construct(output, *first);
      }
      return output;
    }

    template <typename It>
    iterator move_uninit(It first, It last, iterator output) noexcept {
      for (; first != last; ++output, ++first) {
        allocator_.construct(output, std::move(*first));
      }
      return output;
    }

    void fill_uninit(iterator first, iterator last, const T& value) {
      range_check(first, last);
      for (; first != last; ++first) {
        allocator_.construct(first, value);
      }
    }

    void destroy(iterator first, iterator last) noexcept {
      range_check(first, last);
      for (; first != last; ++first) {
        allocator_.destroy(first);
      }
    }

    void delete_all() noexcept {
      destroy(begin(), end());
      allocator_.deallocate(buf_, capacity_);
    }

    void relocate(std::size_t capacity) {
      T* buf = allocator_.allocate(capacity);
      move_uninit(begin(), end(), buf);
      delete_all();
      buf_ = buf;
      capacity_ = capacity;
    }

    template <std::input_iterator It>
    void insert_relocate(iterator pos, It first, It last, std::size_t insertSize) {
      //move to a larger buffer
      T* buf = allocator_.allocate(insertSize);
      move_uninit(pos, end(), copy_uninit(first, last, move_uninit(begin(), pos, buf)));

      //update
      delete_all();
      buf_ = buf;
      capacity_ = insertSize;
      size_ = insertSize;
    }

  public:
    constexpr Vector() = default;

    constexpr Vector(const Vector<T>& rhs)
      : Vector(rhs.begin(), rhs.end()) {
    }

    constexpr Vector(Vector<T>&& rhs) noexcept
      : allocator_(std::exchange(rhs.allocator_, Allocator<T>())),
        size_(std::exchange(rhs.size_, 0)), 
        capacity_(std::exchange(rhs.capacity_, 0)),
        buf_(std::exchange(rhs.buf_, nullptr)) {
    }

    constexpr Vector(std::initializer_list<T> list)
      : Vector(list.begin(), list.end()) {
    }

    template <std::input_iterator It>
    explicit constexpr Vector(It first, It last)
      : allocator_(Allocator<T>()), 
        size_(std::distance(first, last)), 
        capacity_(size_),
        buf_(allocator_.allocate(size_)) {
      copy_uninit(first, last, begin());
    }

    explicit constexpr Vector(std::size_t count, const T& value = T{})
      : allocator_(Allocator<T>()),
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

    constexpr std::size_t size() const {
      return size_;
    }

    constexpr std::size_t capacity() const {
      return capacity_;
    }

    constexpr bool empty() const {
      return size_ == 0;
    }

    void clear() const {
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

    void reserve(std::size_t capacity) {
      if (capacity_ < capacity) {
        relocate(capacity);
      }
    }

    void resize(std::size_t size) {
      if (size_ < size) {
        //expand
        if (capacity_ < size) {
          relocate(size);
        }
        fill_uninit(begin() + size_, begin() + size, T{});
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
        relocate(capacity_ * 2 + 1);
      }
      allocator_.construct(&buf_[size_], std::forward<Args>(args)...);
      ++size_;
    }

    void pop_back() noexcept {
      assert(size_ > 0 && "can not pop back from an empty vector");
      --size_;
      allocator_.destroy(end());
    }

    void erase(iterator first, iterator last) {
      range_check(first, last);
      //replace the erased elements then destroy the rest
      const iterator endIt = end();
      destroy(std::move(last, endIt, first), endIt);
      size_ -= last - first;
    }

    void erase(iterator pos) {
      erase(pos, pos + 1);
    }

    template <std::input_iterator It>
    void insert(iterator pos, It first, It last) {
      range_check(pos, pos);
      assert(first <= last && "first iterator denotes a position after last iterator");

      //special case: directly append to the vector end without reallocation.
      //reallocation must not occur, otherwise it may invalidate the input iterators if self-inserting.
      const std::size_t requiredSize = size_ + std::distance(first, last);
      if (pos == end() && requiredSize <= capacity_) {
        copy_uninit(first, last, end());
        size_ = requiredSize;
        return;
      }

      //otherwise, allocate new buffer so we don't need to check for self-inserting or other edge cases.
      insert_relocate(pos, first, last, requiredSize);
    }

    void insert(iterator pos, const T& value) {
      insert(pos, &value, &value + 1);
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
      for (const T& val : *this) {
        fn(val);
      }
    }

    template <typename CallbackFn>
    void for_each(const CallbackFn& fn) {
      for (T& val : *this) {
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

    typename flow::Vector<T, A>::const_iterator left = lhs.begin();
    typename flow::Vector<T, A>::const_iterator end = lhs.end();
    typename flow::Vector<T, B>::const_iterator right = rhs.begin();
    for (; left != end; ++left, ++right) {
      if (*left != *right) {
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
    for (const std::string& token : tokens) {
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
