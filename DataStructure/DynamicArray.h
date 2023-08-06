#pragma once
#include <iostream>
#include <cassert>

template <typename T>
class DynamicArray;

template <typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <typename T>
bool operator!=(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);


template <typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs);


template <typename T>
class DynamicArray {

public:
  using Type = T;


private:
  T* begin_;
  T* end_;
  T* capacity_;

public:

  /// <summary>
  /// Construct a non-null dynamic array with 0 elements and capacity.
  /// </summary>
  DynamicArray();


  /// <summary>
  /// Construct a dynamic array by copying elements denoted by the interval.
  /// </summary>
  /// <param name="begin">"beginning address of the interval, inclusive"</param>
  /// <param name="end">"endding address of the interval, exclusive"</param>
  DynamicArray(const T* begin, const T* end);


  /// <summary>
  /// Construct a dynamic array by filling with the specified value.
  /// </summary>
  /// <param name="count">"number of elements"</param>
  /// <param name="default_value">"default value of each element"</param>
  DynamicArray(std::size_t count, const T& default_value = T());


  /// <summary>
  /// Construct a dynamic array by deep-copying from an existed dynamic array.
  /// </summary>
  /// <param name="array">"dynamic array to copy from"</param>
  DynamicArray(const DynamicArray& array);


  /// <summary>
  /// Construct a dynamic array by std::move from an existed dynamic array.
  /// </summary>
  /// <param name="array">"dynamic array to move from"</param>
  DynamicArray(DynamicArray&& array) noexcept;


  /// <summary>
  /// Destruct the dynamic array by destroying the elements and freeing the memory.
  /// </summary>
  ~DynamicArray();


  /// <summary>
  /// Deep copying from an existed dynamic array.
  /// </summary>
  /// <param name="array">"dynamic array to copy from"</param>
  /// <returns>"this pointer"</returns>
  DynamicArray& operator=(DynamicArray array) noexcept;


  /// <summary>
  /// Return a reference to the element at index "i"
  /// </summary>
  /// <param name="i">"element index"</param>
  /// <returns>"a reference to the element"</returns>
  T& operator[](std::size_t i);


  /// <summary>
  /// Return a const reference to the element at index "i"
  /// </summary>
  /// <param name="i">"element index"</param>
  /// <returns>"a const reference to the element"</returns>
  const T& operator[](std::size_t i) const;


  /// <summary>
  /// Check if the dynamic array has no element.
  /// </summary>
  /// <returns>true if empty</returns>
  bool Empty() const;


  /// <summary>
  /// Get the number of elements in the dynamic array.
  /// </summary>
  /// <returns>number of elements</returns>
  std::size_t Size() const;


  /// <summary>
  /// Get the capacity of the dynamic array.
  /// </summary>
  /// <returns>the maximum number of elements can contain before requiring reallocation</returns>
  std::size_t Capacity() const;

  /// <summary>
  /// Return a pointer to the first element
  /// </summary>
  /// <returns>a pointer to the first element</returns>
  T* begin();


  /// <summary>
  /// Return a const pointer to the first element
  /// </summary>
  /// <returns>a const pointer to the first element</returns>
  const T* begin() const;


  /// <summary>
  /// Return a pointer to one past the last element
  /// </summary>
  /// <returns>a pointer to one past the last element</returns>
  T* end();


  /// <summary>
  /// Return a const pointer to one past the last element
  /// </summary>
  /// <returns>a const pointer to one past the last element</returns>
  const T* end() const;


  /// <summary>
  /// Return a reference to the first element.
  /// </summary>
  /// <returns>a reference to the first element.</returns>
  T& Front();


  /// <summary>
  /// Return a const reference to the first element.
  /// </summary>
  /// <returns>a const reference to the first element.</returns>
  const T& Front() const;


  /// <summary>
  /// Return a reference to the last element.
  /// </summary>
  /// <returns>a reference to the last element.</returns>
  T& Back();


  /// <summary>
  /// Return a const reference to the last element.
  /// </summary>
  /// <returns>a const reference to the last element.</returns>
  const T& Back() const;


  /// <summary>
  /// <para>Append an element to the end of the dynamic array.</para>
  /// <para>Reallocation is required if exceeds the capacity/</para>
  /// </summary>
  /// <typeparam name="...Args">type</typeparam>
  /// <param name="...args">"element or the constructor arguments"</param>
  template <typename ...Args>
  void PushBack(Args&&... args);


  /// <summary>
  /// <para>Destroy the last element in the dynamic array.</para>
  /// <para>This does not affect the capacity.</para>
  /// </summary>
  void PopBack();


  /// <summary>
  /// <para>Insert elements into the dynamic array.</para>
  /// </summary>
  /// <param name="pos">: position to insert</param>
  /// <param name="first">: the begin address of the elements array to be inserted, inclusive</param>
  /// <param name="last">: the end address of the elements array to be inserted, exclusive</param>
  void Insert(T* pos, const T* first, const T* last);


  /// <summary>
  /// <para>Insert elements into the dynamic array.</para>
  /// </summary>
  /// <param name="pos">: position to insert</param>
  /// <param name="count">: number of elements to insert</param>
  /// <param name="value">: value of each elements</param>
  void Insert(T* pos, std::size_t count, const T& value = T{});


  /// <summary>
  /// <para>Destroy all the elements in the dynamic array.</para>
  /// <para>This does not affect the capacity.</para>
  /// </summary>
  void Clear();


  /// <summary>
  /// <para>Resize the dynamic array to the given size.</para>
  /// <para>If the new size is greater than the old size, then filled with the specified value.</para>
  /// <para>Reallocation is required if exceed the capacity.</para>
  /// <para>If the new size is less than the old size, then the extra elements are destroyed.</para>
  /// <para>This does not affect the capacity.</para>
  /// </summary>
  /// <param name="new_size">"new number of elements"</param>
  /// <param name="default_value">"value for the newly constructed elements"</param>
  void Resize(std::size_t new_size, const T& default_value = T());


  /// <summary>
  /// <para>Reserve the dynamic array to the given capacity.</para>
  /// <para>The new capacity must be equal or greater than the current size.</para>
  /// <para>Reallocation is required.</para>
  /// </summary>
  /// <param name="new_capacity">"new capacity"</param>
  void Reserve(std::size_t new_capacity);


  /// <summary>
  /// <para>Reduce the capacity of the dynamic array to the number of elements it contains.</para>
  /// <para>Reallocation is required.</para>
  /// </summary>
  void ShrinkToFit();

  #ifdef _DEBUG
  void Log() const {
    for (auto b = begin_; b != end_; ++b) {
      std::cout << *b << ' ';
    }
    for (auto b = end_; b != capacity_; ++b) {
      std::cout << *b << "? ";
    }
    std::cout << '\n';
    std::cout << "size: " << Size() << '\n';
    std::cout << "capacity: " << Capacity() << '\n';
  }
  #endif


private:
  /// <summary>
  /// Reallocate the dynamic array with a magic capacity number, so it can reuse the memory.
  /// </summary>
  void Grow();


public:
  /// <summary>
  /// <para>Map all elements specified by the mapping function.</para>
  /// </summary>
  /// <typeparam name="MappingFn"></typeparam>
  /// <param name="map_fn">: a unary callable that maps input to output</param>
  /// <returns>A copy of the mapped dynamic array</returns>
  template <typename MappingFn>
  auto Map(MappingFn map_fn) const->DynamicArray<std::invoke_result_t<MappingFn, const T&>>;


  /// <summary>
  /// <para>Apply the function to all elements.</para>
  /// </summary>
  /// <typeparam name="ForEachFn"></typeparam>
  /// <param name="foreach_fn">: a unary callable</param>
  template <typename ForEachFn>
  void ForEach(ForEachFn foreach_fn);


  /// <summary>
  /// <para>Return a dynamic array contains all the elements that the filter function evaluates to true.</para>
  /// </summary>
  /// <typeparam name="FilterFn"></typeparam>
  /// <param name="filter_fn">: a unary prediate function that returns a bool.</param>
  /// <returns>A copy of the filtered dynamic array.</returns>
  template <typename FilterFn>
  requires std::is_same_v<std::invoke_result_t<FilterFn, const T&>, bool>
  DynamicArray Filter(FilterFn filter_fn) const;


  /// <summary>
  /// <para>Swap the elements between left-hand side and right-hand side array.</para>
  /// </summary>
  /// <typeparam name="T"></typeparam>
  friend void swap<T>(DynamicArray& lhs, DynamicArray& rhs);


  /// <summary>
  /// <para>Compare the equality of left-hand side array and right-hand side array.</para>
  /// </summary>
  /// <param name="lhs">: left-hand side array</param>
  /// <param name="rhs">: right-hand side array</param>
  /// <returns>True if both array have the same size and contain the same elements.</returns>
  friend bool operator==<T>(const DynamicArray& lhs, const DynamicArray& rhs);


  /// <summary>
  /// <para>Compare the equality of left-hand side array and right-hand side array.</para>
  /// </summary>
  /// <param name="lhs">: left-hand side array</param>
  /// <param name="rhs">: right-hand side array</param>
  /// <returns>False if both array have the same size and contain the same elements.</returns>
  friend bool operator!=<T>(const DynamicArray& lhs, const DynamicArray& rhs);
};


template <typename T>
DynamicArray<T>::DynamicArray() : 
  begin_(static_cast<T*>(operator new[](0))), end_(begin_), capacity_(begin_) {}

template <typename T>
DynamicArray<T>::DynamicArray(const T* begin, const T* end) {

  assert(begin && end && begin <= end);

  //allocate the array
  const std::size_t count = end - begin;
  begin_ = static_cast<T*>(operator new[](count * sizeof(T)));
  end_ = begin_ + count;
  capacity_ = end_;

  //copying the elements
  for (T* addr = begin_; addr != end_; ++addr, ++begin) {
    new(addr) T(*begin);
  }
}

template <typename T>
DynamicArray<T>::DynamicArray(std::size_t count, const T& value) {

  assert(count >= 0);

  //allocate the array
  begin_ = static_cast<T*>(operator new[](count * sizeof(T)));
  end_ = begin_ + count;
  capacity_ = end_;

  //copying the elements
  for (T* addr = begin_; addr != end_; ++addr) {
    new(addr) T(value);
  }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& array) : 
  DynamicArray(array.begin(), array.end()) {}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& array) noexcept :
  begin_(std::exchange(array.begin_, nullptr)), end_(std::exchange(array.end_, nullptr)), capacity_(std::exchange(array.capacity_, nullptr)) {}

template <typename T>
DynamicArray<T>::~DynamicArray() {

  //destroy the elements
  for (T* addr = begin_; addr != end_; ++addr) {
    addr->~T();
  }

  //free the memory
  operator delete[](begin_);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray array) noexcept {
  swap(*this, array);
  return *this;
}

template <typename T>
T& DynamicArray<T>::operator[](std::size_t i) {
  return const_cast<T&>(static_cast<const DynamicArray&>(*this).operator[](i));
}

template <typename T>
const T& DynamicArray<T>::operator[](std::size_t i) const {
  assert(begin_ + i < end_);
  return begin_[i];
}

template <typename T>
bool DynamicArray<T>::Empty() const {
  return begin_ == end_;
}

template <typename T>
std::size_t DynamicArray<T>::Size() const {
  return end_ - begin_;
}

template <typename T>
std::size_t DynamicArray<T>::Capacity() const {
  return capacity_ - begin_;
}

template <typename T>
T* DynamicArray<T>::begin() {
  return const_cast<T*>(static_cast<const DynamicArray&>(*this).begin());
}

template <typename T>
const T* DynamicArray<T>::begin() const {
  return begin_;
}

template <typename T>
T* DynamicArray<T>::end() {
  return const_cast<T*>(static_cast<const DynamicArray&>(*this).end());
}

template <typename T>
const T* DynamicArray<T>::end() const {
  return end_;
}

template <typename T>
T& DynamicArray<T>::Front() {
  return const_cast<T&>(static_cast<const DynamicArray&>(*this).Front());
}

template <typename T>
const T& DynamicArray<T>::Front() const {
  assert(!Empty());
  return *begin_;
}

template <typename T>
T& DynamicArray<T>::Back() {
  return const_cast<T&>(static_cast<const DynamicArray&>(*this).Back());
}

template <typename T>
const T& DynamicArray<T>::Back() const {
  assert(!Empty());
  return *(end_ - 1);
}


template <typename T>
template <typename ...Args>
void DynamicArray<T>::PushBack(Args&&... args) {

  //reallocate if exceed capacity
  if (end_ == capacity_) {
    Grow();
  }

  //append the new element
  new(end_++) T(std::forward<Args>(args)...);
}

template <typename T>
void DynamicArray<T>::PopBack() {
  assert(!Empty());
  assert(begin_ <= end_);
  (--end_)->~T();
}

template<typename T>
void DynamicArray<T>::Insert(T* pos, const T* first, const T* last) {
  assert(first <= last);
  assert(begin_ <= pos && pos <= end_);

  const std::size_t insert_index = pos - begin_;
  const std::size_t insert_size = last - first;

  //allocate new buffer
  DynamicArray<T> result_array;
  result_array.Reserve(Size() + insert_size);
  result_array.end_ = result_array.capacity_;

  //copy the inserted elements
  for (T* node = result_array.begin_ + insert_index; first != last; ++first, ++node) {
    new(node) T(*first);
  }

  //move the elements before insertion interval
  for (T* begin = begin_, *node = result_array.begin_; begin != pos; ++begin, ++node) {
    new(node) T(std::move(*begin));
  }

  //move the elements after insertion interval
  for (T* begin = begin_ + insert_index, *node = result_array.begin_ + insert_index + insert_size; begin != end_; ++begin, ++node) {
    new(node) T(std::move(*begin));
  }

  swap(*this, result_array);
}

template<typename T>
void DynamicArray<T>::Insert(T* pos, std::size_t count, const T& value){
  assert(begin_ <= pos && pos <= end_);

  const std::size_t insert_index = pos - begin_;
  const std::size_t insert_size = count;

  DynamicArray<T> result_array;
  result_array.Reserve(Size() + insert_size);
  result_array.end_ = result_array.capacity_;

  //copy the inserted elements
  for (T* node = result_array.begin_ + insert_index; count > 0; --count, ++node) {
    new(node) T(value);
  }

  //move the elements before insertion interval
  for (T* begin = begin_, *node = result_array.begin_; begin != pos; ++begin, ++node) {
    new(node) T(std::move(*begin));
  }

  //move the elements after insertion interval
  for (T* begin = begin_ + insert_index, *node = result_array.begin_ + insert_index + insert_size; begin != end_; ++begin, ++node) {
    new(node) T(std::move(*begin));
  }

  swap(*this, result_array);
}

template <typename T>
void DynamicArray<T>::Clear() {

  assert(begin_ <= end_);

  //destroy the elements
  while (end_ != begin_) {
    (--end_)->~T();
  }
}

template <typename T>
void DynamicArray<T>::Resize(std::size_t new_size, const T& default_value) {

  const std::size_t old_sz = Size();

  //appending elements
  if (old_sz < new_size) {

    //check if exceed capacity
    if (Capacity() < new_size) {
      Reserve(new_size);
    }

    //construct new elements
    const T* new_end = begin_ + new_size;
    while (end_ != new_end) {
      new(end_++) T(default_value);
    }
  } 

  //destroying elements
  else {
    const T* new_end = begin_ + new_size;
    while (end_ != new_end) {
      (--end_)->~T();
    }
  }
}

template <typename T>
void DynamicArray<T>::Reserve(std::size_t new_capacity) {

  //compare to the size, because shrink to fit may reduce the capacity
  if (Size() <= new_capacity) {

    //save old size
    const std::size_t old_sz = Size();

    //allocate new memory
    T* new_mem = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

    //realloate elements
    for (T* srce = begin_, *node = new_mem; srce != end_; ++srce, ++node) {
      new(node) T(std::move(*srce));
    }

    //destroy old elements
    for (T* addr = begin_; addr != end_; ++addr) {
      addr->~T();
    }

    //destroy the old memory
    operator delete[](begin_);
    
    //update the pointer
    begin_ = new_mem;
    end_ = new_mem + old_sz;
    capacity_ = new_mem + new_capacity;
  }
}

template <typename T>
void DynamicArray<T>::ShrinkToFit() {
  Reserve(Size());
}

template <typename T>
void DynamicArray<T>::Grow() {

  //+1 to avoid 0 * x == 0 issue
  //magic number close to golden ratio, allows to reuse previously allocated memory
  Reserve(static_cast<std::size_t>(Capacity() * 3 / 2 + 1));
}





template<typename T>
template<typename MappingFn>
auto DynamicArray<T>::Map(MappingFn map_fn) const -> DynamicArray<std::invoke_result_t<MappingFn, const T&>> {
  using MappedType = std::invoke_result_t<MappingFn, const T&>;

  DynamicArray<MappedType> mapped;
  mapped.Reserve(Size());

  //apply mapping function to each element
  for (const T& obj : *this) {
    mapped.PushBack(map_fn(obj));
  }
  return mapped;
}

template<typename T>
template<typename ForEachFn>
void DynamicArray<T>::ForEach(ForEachFn foreach_fn) {

  //apply function to each element
  for (auto& obj : *this) {
    foreach_fn(obj);
  }
}

template<typename T>
template<typename FilterFn>
  requires std::is_same_v<std::invoke_result_t<FilterFn, const T&>, bool>
DynamicArray<T> DynamicArray<T>::Filter(FilterFn filter_fn) const {
  DynamicArray<T> filtered;

  //keep the element if filter function evaluates to true
  for (const auto& obj : *this) {
    if (filter_fn(obj)) {
      filtered.PushBack(obj);
    }
  }
  return filtered;
}









template <typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) {

  //swap internal pointers
  using std::swap;
  swap(lhs.begin_, rhs.begin_);
  swap(lhs.end_, rhs.end_);
  swap(lhs.capacity_, rhs.capacity_);
}

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
  if (lhs.Size() != rhs.Size()) return false;
  for (const T* left = lhs.begin_, *right = rhs.begin_; left != lhs.end_; ++left, ++right) {
    if (*left != *right) return false;
  }
  return true;
}

template<typename T>
bool operator!=(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
  return !(lhs == rhs);
}