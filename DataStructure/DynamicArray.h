#pragma once
#include <iostream>
#include <algorithm>
#include <cassert>
#include "io.h"

template<typename T>
class DynamicArray;

template<typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs);

template <typename T>
class DynamicArray
{
    T* data_;
    int capacity_;
    int size_;

public:

    using value_type = T;

    DynamicArray();
    DynamicArray(std::initializer_list<T> lst);
    explicit DynamicArray(const T* arr, int size);
    explicit DynamicArray(const T* begin, const T* end);
    explicit DynamicArray(int new_size, const T& val = T());
    DynamicArray(const DynamicArray& rhs);
    DynamicArray(DynamicArray&& rhs) noexcept;
    ~DynamicArray();

    DynamicArray& operator=(DynamicArray rhs);

    T& operator[](int r);
    const T& operator[](int r) const;


    //basic container operators
    bool IsEmpty() const;
    int Capacity() const;
    int Size() const;


    //specialized container operators
    T& At(int r);
    const T& At(int r) const;
    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;
    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;



    void PushBack(const T& val);
    void PushBack(T&& val);
    void PopBack();
    void Clear();


    //custom memory management
    void Resize(int new_size, const T& val = T());
    void Reserve(int new_capacity);
    void ShrinkToFit();
    T* Data();
    const T* Data() const;

    friend void swap<T>(DynamicArray<T>& lhs, DynamicArray<T>& rhs);
};

template<typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs)
{
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.capacity_, rhs.capacity_);
    swap(lhs.size_, rhs.size_);
}


template <typename T>
DynamicArray<T>::DynamicArray() : 
    data_(static_cast<T*>(operator new[](sizeof(T)))), capacity_(1), size_(0)
{
    //empty
}

template <typename T>
DynamicArray<T>::DynamicArray(const T* arr, int size) :
    data_(static_cast<T*>(operator new[](size * sizeof(T)))), capacity_(static_cast<int>(size)), size_(static_cast<int>(size))
{
    assert(size >= 0);
    std::uninitialized_copy(arr, arr + size, data_);
}

template <typename T>
DynamicArray<T>::DynamicArray(const T* begin, const T* end) : DynamicArray(begin, static_cast<int>(end - begin)) 
{
    assert(begin <= end);
}

template <typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> lst) : DynamicArray(lst.begin(), static_cast<int>(lst.size())) 
{
    cug::io::Log("init list\n");
    assert(lst.size() >= 0);
}

template <typename T>
DynamicArray<T>::DynamicArray(int new_size, const T& val) : 
    data_(static_cast<T*>(operator new[](new_size * sizeof(T)))), capacity_(new_size), size_(new_size)
{
    cug::io::Log("filled \n");
    assert(new_size >= 0);

    //std::fill calls operator=, which is undefined behavior when used on uninitialized object)
    //alternative: loop + placement new
    std::uninitialized_fill(data_, data_ + size_, val);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& rhs) : 
    data_(static_cast<T*>(operator new[](rhs.capacity_ * sizeof(T)))), capacity_(rhs.capacity_), size_(rhs.size_)
{
    assert(rhs.capacity_ >= 0);

    //std::copy calls operator=, which is undefined behavior when used on uninitialized object)

    //alternative: loop + placement new
    std::uninitialized_copy(rhs.data_, rhs.data_ + rhs.size_, data_);
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& rhs) noexcept :
    data_(std::exchange(rhs.data_, nullptr)), capacity_(std::exchange(rhs.capacity_, 0)), size_(std::exchange(rhs.size_, 0)) {}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    //destruct objects
    std::destroy(data_, data_ + size_);

    //free the memory
    operator delete[](data_);
}


//operators overloading
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T> rhs)
{
    swap(*this, rhs);
    return *this;
}

template <typename T>
T& DynamicArray<T>::operator[](int r)
{
    assert(0 <= r && r < size_);
    return data_[r];
}

template <typename T>
const T& DynamicArray<T>::operator[](int r) const
{
    assert(0 <= r && r < size_);
    return data_[r];
}





//basic container operators
template <typename T>
bool DynamicArray<T>::IsEmpty() const
{
    return size_ == 0;
}

template<typename T>
int DynamicArray<T>::Capacity() const
{
    return capacity_;
}

template <typename T>
int DynamicArray<T>::Size() const
{
    return size_;
}



//specialized container operators
template <typename T>
T& DynamicArray<T>::At(int r)
{
    if (r < 0 || r >= size_) throw std::out_of_range("index out of bound");
    else return data_[r];
}

template <typename T>
const T& DynamicArray<T>::At(int r) const
{
    if (r < 0 || r >= size_) throw std::out_of_range("index out of bound");
    else return data_[r];
}

template <typename T>
T& DynamicArray<T>::Front()
{
    assert(!this->IsEmpty());
    return data_[0];
}

template <typename T>
const T& DynamicArray<T>::Front() const
{
    assert(!this->IsEmpty());
    return data_[0];
}

template <typename T>
T& DynamicArray<T>::Back()
{
    assert(!this->IsEmpty());
    return data_[size_ - 1];
}

template <typename T>
const T& DynamicArray<T>::Back() const
{
    assert(!this->IsEmpty());
    return data_[size_ - 1];
}

template <typename T>
T* DynamicArray<T>::begin()
{
    return data_;
}
template <typename T>
const T* DynamicArray<T>::begin() const
{
    return data_;
}
template <typename T>
T* DynamicArray<T>::end()
{
    return data_ + size_;
}
template <typename T>
const T* DynamicArray<T>::end() const
{
    return data_ + size_;
}





template <typename T>
void DynamicArray<T>::PushBack(const T& val)
{
    //check if exceed capacity
    if (size_ >= capacity_) Reserve(2 * capacity_);

    //copy construct
    new(data_ + size_) T(val);
    ++size_;
}

template <typename T>
void DynamicArray<T>::PushBack(T&& val)
{
    //check if exceed capacity
    if (size_ >= capacity_) Reserve(2 * capacity_);

    //move construct
    new(data_ + size_) T(std::move(val));
    ++size_;
}

template <typename T>
void DynamicArray<T>::PopBack()
{
    data_[--size_].~T();
}

template <typename T>
void DynamicArray<T>::Clear()
{
    //destroy the objects
    std::destroy(data_, data_ + size_);
    size_ = 0;
}

//custom memory management
template <typename T>
void DynamicArray<T>::Resize(int new_size, const T& val)
{
    assert(new_size >= 0);

    //shrinking
    if (new_size < size_)
    {
        //destroy truncated objects
        std::destroy(data_ + new_size, data_ + size_);
    }
    else 
    {
        //expand capacity if appropriate
        Reserve(new_size);

        //fill with the new objects
        std::uninitialized_fill(data_ + size_, data_ + new_size, val);
    }

    size_ = new_size;
}

template <typename T>
void DynamicArray<T>::Reserve(int new_capacity)
{
    if (new_capacity > capacity_)
    {
        //allocate heap with increased capacity
        T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

        //move in the old data
        std::uninitialized_move(data_, data_ + size_, new_data);

        //destroy the old data, free the old heap
        std::destroy(data_, data_ + size_);
        operator delete[](data_);

        //update the container
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

template <typename T>
void DynamicArray<T>::ShrinkToFit()
{
    //allocate heap with size-equivalent capacity
    T* new_data = static_cast<T*>(operator new[](size_ * sizeof(T)));

    //move in the old data
    std::uninitialized_move(data_, data_ + size_, new_data);

    //destroy the old data, free the old heap
    std::destroy(data_, data_ + size_);
    operator delete[](data_);

    //update the container
    data_ = new_data;
    capacity_ = size_;
}

template <typename T>
T* DynamicArray<T>::Data()
{
    return data_;
}

template <typename T>
const T* DynamicArray<T>::Data() const
{
    return data_;
}