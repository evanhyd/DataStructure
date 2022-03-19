#pragma once
#include <algorithm>
#include <cassert>
#include "debug.h"

#define DEBUG_MODE true


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
    DynamicArray(int new_size, const T& val = T());
    DynamicArray(const DynamicArray& rhs);
    DynamicArray(DynamicArray&& rhs) noexcept;
    ~DynamicArray();

    DynamicArray& operator=(DynamicArray rhs);

    T& operator[](int i);
    const T& operator[](int i) const;


    //basic container operators
    bool IsEmpty() const;
    int Capacity() const;
    int Size() const;


    //specialized container operators
    T& At(int i);
    const T& At(int i) const;
    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;
    void PushBack(const T& val);
    void PushBack(T&& val);
    void PopBack();
    void Clear();


    //custom memory management
    void Resize(int new_size, const T& val = T());
    void Reserve(int new_capacity);
    void ShrinkToFit();


private:
    void ClearObj();
    void ClearMem();


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
DynamicArray<T>::DynamicArray() : data_(static_cast<T*>(operator new(sizeof(T)))), capacity_(1), size_(0)
{
    //empty
}

template <typename T>
DynamicArray<T>::DynamicArray(int new_size, const T& val) : 
    data_(static_cast<T*>(operator new(new_size * sizeof(T)))), capacity_(new_size), size_(new_size)
{
    //fill the default value [bug](fill calls operator=, which can be undefined behavior when uses on uninitialized object)
    //std::fill(data_, data_ + size_, val);

    //alternative: loop + placement new
    std::uninitialized_fill(data_, data_ + size_, val);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& rhs) : 
    data_(static_cast<T*>(operator new(rhs.capacity_ * sizeof(T)))), capacity_(rhs.capacity_), size_(rhs.size_)
{
    //copy the constructed objects [bug](copy calls operator=, which can be undefined behavior when uses on uninitialized object)
    //std::copy(rhs.data_, rhs.data_ + rhs.size_, data_);

    //alternative: loop + placement new
    std::uninitialized_copy(rhs.data_, rhs.data_ + rhs.size_, data_);
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& rhs) noexcept : DynamicArray()
{
    swap(*this, rhs);
}

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
T& DynamicArray<T>::operator[](int i)
{
    return data_[i];
}

template <typename T>
const T& DynamicArray<T>::operator[](int i) const
{
    return data_[i];
}





//basic container operators
template <typename T>
bool DynamicArray<T>::IsEmpty() const
{
    return size_ == 0;
}

template <typename T>
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
T& DynamicArray<T>::At(int i)
{
    if (i < 0 || i >= size_) throw std::out_of_range("index out of bound");
    else return data_[i];
}

template <typename T>
const T& DynamicArray<T>::At(int i) const
{
    if (i < 0 || i >= size_) throw std::out_of_range("index out of bound");
    else return data_[i];
}

template <typename T>
T& DynamicArray<T>::Front()
{
    return data_[0];
}

template <typename T>
const T& DynamicArray<T>::Front() const
{
    return data_[0];
}

template <typename T>
T& DynamicArray<T>::Back()
{
    return data_[size_ - 1];
}

template <typename T>
const T& DynamicArray<T>::Back() const
{
    return data_[size_ - 1];
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
        std::uninitialized_fill(data_ + size_, data_ + new_size, T());
    }

    size_ = new_size;
}

template <typename T>
void DynamicArray<T>::Reserve(int new_capacity)
{
    if (new_capacity > capacity_)
    {
        //allocate heap with increased capacity
        T* new_data = static_cast<T*>(operator new(new_capacity * sizeof(T)));

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
    T* new_data = operator new(size_ * sizeof(T));

    //move in the old data
    std::uninitialized_move(data_, data_ + size_, new_data);

    //destroy the old data, free the old heap
    std::destroy(data_, data_ + size_);
    operator delete[](data_);

    //update the container
    data_ = new_data;
    capacity_ = size_;
}