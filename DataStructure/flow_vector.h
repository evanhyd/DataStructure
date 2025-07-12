#pragma once
#include "flow_counted_value_view_iterator.h"
#include "flow_memory_algorithm.h"
#include "flow_polymorphic_allocator.h"
#include <cassert>
#include <concepts>
#include <initializer_list>
#include <memory>
#include <utility>

namespace flow {

  template <typename Strategy>
  concept GrowthStrategy = requires(Strategy strategy, size_t num) {
    { strategy(num) } -> std::same_as<std::size_t>;
  };

  struct VectorGrowthStrategy {
    struct GoldenExpand {
      std::size_t operator()(std::size_t oldCapacity) const {
        return oldCapacity + oldCapacity / 2 + 1;
      }
    };

    struct DoubleExpand {
      std::size_t operator()(std::size_t oldCapacity) const {
        return oldCapacity * 2 + 1;
      }
    };

    struct FibonacciExpand {
      std::size_t n1 = 0;
      std::size_t n2 = 1;
      std::size_t operator()(std::size_t oldCapacity) {
        do {
          std::size_t n3 = n1 + n2;
          n1 = n2;
          n2 = n3;
        } while (n2 <= oldCapacity);
        return n2;
      }
    };
  };


  template <typename T, typename Allocator = PolymorphicAllocator<>, GrowthStrategy Strategy = VectorGrowthStrategy::GoldenExpand>
  class Vector {
  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<T>;

  private:
    using allocator_trait = std::allocator_traits<Allocator>::template rebind_traits<T>;

    allocator_type allocator_;
    Strategy growthStrategy_;
    std::size_t size_;
    std::size_t capacity_;
    T* buffer_;

    // Clean up the old buffer.
    void updateBuffer(T* buffer, std::size_t capacity) {
      deleteBuffer(allocator_, buffer_, size_, capacity_);
      buffer_ = buffer;
      capacity_ = capacity;
    }

    // Allocate a new buffer with the capacity, and relocate the elements to it.
    // This update the buffer_ and capacity_ internally.
    void relocateBuffer(std::size_t capacity) {
      assert(capacity_ < capacity && "new capacity is no larger than the old capacity");

      T* buffer = allocator_trait::allocate(allocator_, capacity);
      uninitializedMove(allocator_, begin(), end(), buffer);
      updateBuffer(buffer, capacity);
    }

    // Allocate a new buffer with the capacity, and relocate the elements to it with a hole at pos.
    void relocateBufferWithHoles(std::size_t capacity, iterator pos, std::size_t holeSize) {
      assert(capacity_ < capacity && "new capacity is no larger than the old capacity");

      T* buffer = allocator_trait::allocate(allocator_, capacity);
      iterator leftHalf = uninitializedMove(allocator_, begin(), pos, buffer); // Move left half.
      uninitializedMove(allocator_, pos, end(), leftHalf + holeSize); // Move right half.
      updateBuffer(buffer, capacity);
    }

    template <typename ...U>
    void resizeImp(std::size_t size, const U&... optionalValue) {
      static_assert(sizeof...(optionalValue) <= 1, "no fill value or exactly one copy");
      if (size_ < size) {
        // Relocate if not enough capacity.
        if (capacity_ < size) {
          relocateBuffer(size);
        }
        uninitializedEmplace(allocator_, buffer_ + size_, buffer_ + size, optionalValue...);

      } else if (size < size_) {
        // Shrink.
        destroyElements(allocator_, buffer_ + size, buffer_ + size_);
      }
      size_ = size;
    }

  public:
    // Constructors
    // Default construct should not be explicit.
    constexpr Vector() noexcept
      : Vector(allocator_type{}) {
    }

    explicit constexpr Vector(const allocator_type& allocator)
      : allocator_(allocator),
        growthStrategy_(),
        size_(0),
        capacity_(0),
        buffer_(nullptr) {
    }

    constexpr Vector(const Vector& rhs)
      : Vector(rhs, rhs.get_allocator()) {
    }

    constexpr Vector(const Vector& rhs, const allocator_type& allocator)
      : allocator_(allocator),
        growthStrategy_(rhs.growthStrategy_),
        size_(rhs.size_),
        capacity_(rhs.capacity_),
        buffer_(allocator_trait::allocate(allocator_, rhs.capacity_)) {
      uninitializedForward(allocator_, rhs.begin(), rhs.end(), buffer_);
    }

    // Move constructor must be noexcept
    constexpr Vector(Vector&& rhs) noexcept
      : Vector(std::move(rhs), rhs.get_allocator()) {
    }

    // Extended move constructor may through
    constexpr Vector(Vector&& rhs, const allocator_type& allocator)
      : Vector(allocator) {

      if (rhs.get_allocator() == allocator) {
        swap(*this, rhs);
      } else {
        // Create a copy with new allocator, then swap over.
        Vector rhsCopy(rhs, allocator);
        swap(*this, rhsCopy);
      }
    }

    template <std::input_iterator It>
    explicit constexpr Vector(It first, It last, const allocator_type& allocator = {})
      : allocator_(allocator),
        growthStrategy_(),
        size_(std::distance(first, last)),
        capacity_(size_),
        buffer_(allocator_trait::allocate(allocator_, capacity_)) {
      uninitializedForward(allocator_, first, last, buffer_);
    }

    constexpr Vector(std::initializer_list<T> list, const allocator_type& allocator = {})
      : Vector(list.begin(), list.end(), allocator) {
    }

    explicit constexpr Vector(std::size_t count, const allocator_type& allocator = {})
      : allocator_(allocator),
        growthStrategy_(),
        size_(count),
        capacity_(count),
        buffer_(allocator_trait::allocate(allocator_, count)) {
      uninitializedEmplaceN(allocator_, buffer_, count);
    }

    explicit constexpr Vector(std::size_t count, const T& value, const allocator_type& allocator = {})
      : allocator_(allocator),
        growthStrategy_(),
        size_(count),
        capacity_(count),
        buffer_(allocator_trait::allocate(allocator_, count)) {
      uninitializedFillN(allocator_, buffer_, count, value);
    }

    // Destructor.
    ~Vector() {
      deleteBuffer(allocator_, buffer_, size_, capacity_);
    }

    // Operator.
    Vector& operator=(Vector rhs) noexcept {
      swap(*this, rhs);
      return *this;
    }

    T& operator[](std::size_t i) noexcept {
      assert(i < size_ && "index out of bound");
      return buffer_[i];
    }

    const T& operator[](std::size_t i) const noexcept {
      assert(i < size_ && "index out of bound");
      return buffer_[i];
    }

    // Allocator.
    allocator_type get_allocator() const noexcept {
      return allocator_;
    }

    // Iterators
    iterator begin() noexcept {
      return buffer_;
    }

    constexpr const_iterator begin() const noexcept {
      return buffer_;
    }

    iterator end() noexcept {
      return buffer_ + size_;
    }

    constexpr const_iterator end() const noexcept {
      return buffer_ + size_;
    }

    // Accessers.
    constexpr std::size_t size() const noexcept {
      return size_;
    }

    constexpr std::size_t capacity() const noexcept {
      return capacity_;
    }

    constexpr bool empty() const noexcept {
      return size_ == 0;
    }

    T& front() noexcept {
      assert(0 < size_ && "access empty Vector front");
      return buffer_[0];
    }

    constexpr const T& front() const noexcept {
      assert(0 < size_ && "access empty Vector front");
      return buffer_[0];
    }

    T& back() noexcept {
      assert(0 < size_ && "access empty Vector back");
      return buffer_[size_ - 1];
    }

    constexpr const T& back() const noexcept {
      assert(0 < size_ && "access empty Vector back");
      return buffer_[size_ - 1];
    }

    // Mutators
    void clear() noexcept {
      destroyElementsN(allocator_, buffer_, size_);
      size_ = 0;
    }

    void reserve(std::size_t capacity) {
      if (capacity_ < capacity) {
        relocateBuffer(capacity);
      }
    }

    void resize(std::size_t size) {
      resizeImp(size);
    }

    void resize(std::size_t size, const T& value) {
      resizeImp(size, value);
    }
     
    // https://stackoverflow.com/questions/10890653/why-would-i-ever-use-push-back-instead-of-emplace-back
    // TLDR: Consider emplace back an address when constructing unique_ptr.
    void pushBack(const T& value) {
      emplaceBack(value);
    }

    void pushBack(T&& value) {
      emplaceBack(std::move(value));
    }

    template <typename ...Args>
    void emplaceBack(Args&&... args) {
      if (size_ == capacity_) {
        relocateBuffer(growthStrategy_(capacity_));
      }
      allocator_trait::construct(allocator_, buffer_ + size_, std::forward<Args>(args)...);
      ++size_;
    }

    void popBack() noexcept {
      assert(0 < size_ && "can not pop back from an empty vector");
      --size_;
      allocator_trait::destroy(allocator_, buffer_ + size_);
    }

    /// <summary>
    /// Removes the element at the given position.
    /// </summary>
    /// <param name="pos">Iterator to the element to remove (must not be end()).</param>
    /// <returns>Iterator to the next element, or end() if last was removed.</returns>
    iterator erase(iterator pos) noexcept {
      assert(begin() <= pos && pos < end() && "can not erase before begin() or at the end()");
      std::move(pos + 1, end(), pos);
      popBack();
      return pos;
    }

    /// <summary>
    /// Removes elements in the range [first, last).
    /// </summary>
    /// <param name="first">Iterator to the start of the range to remove.</param>
    /// <param name="last">Iterator to the end of the range to remove.</param>
    /// <returns>Iterator to the next element, or end() if range ended at the last element.</returns>
    iterator erase(iterator first, iterator last) noexcept {
      assert(begin() <= first && last <= end() && "can not erase before begin() or past the end()");
      assert(first <= last && "first must be before last");

      iterator afterMoved = std::move(last, end(), first);
      destroyElements(allocator_, afterMoved, end());
      size_ -= last - first;
      return first;
    }

    template <typename ...Args>
    void emplace(iterator pos, Args&&... args) {
      // Special case: append at the end.
      if (pos == end()) {
        emplaceBack(std::forward<Args>(args)...);
        return;
      }

      if (size_ < capacity_) {
        // Enough capacity, right shift the elements.
        // Major optimization to use memcpy, copy_backward, or range move_backward instead of handroll loop. A 70% reduction in computation time.
        allocator_trait::construct(allocator_, end(), std::move(back()));
        std::move_backward(pos, end()-1, end());
        *pos = T(std::forward<Args>(args)...);

      } else {
        // Not enough capacity, relocate all to a new buffer.
        std::size_t index = pos - buffer_;
        relocateBufferWithHoles(growthStrategy_(capacity_), pos, 1);
        allocator_trait::construct(allocator_, buffer_ + index, std::forward<Args>(args)...);
      }
      ++size_;
    }

    void insert(iterator pos, const T& value) {
      emplace(pos, value);
    }

    void insert(iterator pos, T&& value) {
      emplace(pos, std::move(value));
    }

    void insert(iterator pos, std::size_t count, const T& value) {
      insert(pos, CountedValueViewIterator(value, count), CountedValueViewIterator(value));
    }

    // Do not support self inserting.
    template <std::input_iterator It>
    void insert(iterator pos, It first, It last) {
      const std::size_t insertedElementsSize = std::distance(first, last);
      const std::size_t requiredSize = size_ + insertedElementsSize;

      if (capacity_ < requiredSize) {
        // Not enough capacity, relocate to a new buffer.
        std::size_t index = pos - begin();
        std::size_t newCapacity = growthStrategy_(requiredSize);
        relocateBufferWithHoles(newCapacity, pos, insertedElementsSize);
        uninitializedForward(allocator_, first, last, begin() + index);
      } else {
        // Enough capacity, shift elements.
        // Inserted that are inbound = Shifted that are outbound.
        const std::size_t shiftedElementSize = end() - pos;
        const std::size_t conflictedRangeSize = std::min(insertedElementsSize, shiftedElementSize);
        const std::size_t shiftedElementsInboundSize = shiftedElementSize - conflictedRangeSize;
        const std::size_t insertedElementsOutboundSize = insertedElementsSize - conflictedRangeSize;

        // Shifted elements that are on uninitialized buffer range.
        // This range can be empty if insert at end().
        uninitializedMove(allocator_, end() - conflictedRangeSize, end(), pos + insertedElementsSize + shiftedElementsInboundSize);

        // Shifted elements that are on initialized buffer range.
        // This range can be empty if all shifted elements must move to the uninitialized buffer from the previous step.
        std::move_backward(pos, pos + shiftedElementsInboundSize, end());

        // Inserted elements that are on initialized buffer range starting at pos.
        // This range can be empty if insert at end().
        for (std::size_t i = 0; i < conflictedRangeSize; ++i, ++first, ++pos) {
          *pos = *first;
        }

        // Inserted elements that are on uninitialized buffer range.
        // This range can be empty if insertedElementSize <= shiftedElementSize -> conflictedRangeSize = insertedElementSize.
        // Such that there are enough space in the initialized buffer range.
        uninitializedForwardN(allocator_, first, insertedElementsOutboundSize, pos);
      }

      size_ = requiredSize;
    }

    void insert(iterator pos, std::initializer_list<T> list) {
      insert(pos, list.begin(), list.end());
    }

    // Friends.
    friend void swap(Vector& lhs, Vector& rhs) noexcept {
      using std::swap;
      swap(lhs.allocator_, rhs.allocator_);
      swap(lhs.growthStrategy_, rhs.growthStrategy_);
      swap(lhs.size_, rhs.size_);
      swap(lhs.capacity_, rhs.capacity_);
      swap(lhs.buffer_, rhs.buffer_);
    }
  };
}

template <typename T>
bool operator==(const flow::Vector<T>& lhs, const flow::Vector<T>& rhs) noexcept {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator!=(const flow::Vector<T>& lhs, const flow::Vector<T>& rhs) noexcept {
  return !(lhs == rhs);
} 

//namespace nope {
//  template <typename T>
//  class Vector {
//    template<typename MappingFn, template<typename> typename Alloc = Allocator>
//    Vector<std::invoke_result_t<MappingFn, const T&>, Alloc> map(const MappingFn& fn) const {
//      Vector<std::invoke_result_t<MappingFn, const T&>, Alloc> mapped;
//      mapped.relocate(size_);
//      for (const T& val : *this) {
//        mapped.push_back(fn(val));
//      }
//      return mapped;
//    }
//
//    template <typename FilterFn, template<typename> typename Alloc = Allocator>
//    Vector<T, Alloc> filter(const FilterFn& fn) const {
//      static_assert(std::is_same_v<std::invoke_result_t<FilterFn, const T&>, bool>, "filter function must evaluate to bool");
//      Vector<T, Alloc> filtered;
//      for (const T& val : *this) {
//        if (fn(val)) {
//          filtered.push_back(val);
//        }
//      }
//      return filtered;
//    }
//
//    template <typename CallbackFn>
//    void for_each(const CallbackFn& fn) const {
//      for (auto& val : *this) {
//        fn(val);
//      }
//    }
//
//    friend class Vector;
//  };
//
//  
//
//  //TODO: support move semantics
//  template <typename ...Vec>
//  Vector<Tuple<typename Vec::value_type...>> zip(const Vec&... vec) {
//    using ZipType = Tuple<typename Vec::value_type...>;
//    std::size_t minSize = std::min({vec.size()...});
//    Vector<ZipType> zipped;
//    zipped.reserve(minSize);
//    for (std::size_t i = 0; i < minSize; ++i) {
//      zipped.push_back(ZipType{vec[i]...});
//    }
//    return zipped;
//  }
//
//  Vector<std::string> split(const std::string& line, const std::string& delimiter) {
//    if (delimiter.empty()) {
//      return { line };
//    }
//
//    Vector<std::string> tokens;
//    for (std::size_t pos = 0;;) {
//      std::size_t nextPos = line.find(delimiter, pos);
//      tokens.push_back(line.substr(pos, nextPos - pos));
//      if (nextPos == std::string::npos) {
//        break;
//      }
//      pos = nextPos + delimiter.size();
//    }
//    return tokens;
//  }
//
//  std::string join(const Vector<std::string>& tokens, const std::string& separator) {
//    if (tokens.empty()) {
//      return "";
//    }
//
//    std::size_t sz = (tokens.size() - 1) * separator.size();
//    for (auto& token : tokens) {
//      sz += token.size();
//    }
//
//    std::string line;
//    line.reserve(sz);
//    for (std::size_t i = 0; i + 1 < tokens.size(); ++i) {
//      line += tokens[i] + separator;
//    }
//    line += tokens.back();
//    return line;
//  }
//}
