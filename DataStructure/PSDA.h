#pragma once
#include <iostream>
#include <cassert>

template <typename T>
class PSDA;

template <typename T>
void swap(PSDA<T>& lhs, PSDA<T>& rhs);

//prefix sum difference array
template <typename T>
class PSDA
{
    T* data_;
    int size_;

public:

    PSDA(const T* begin, const T* end);
    PSDA(const T* arr, int size);
    PSDA(std::initializer_list<T> lst);
    
    PSDA(const PSDA& other);
    PSDA(PSDA&& other) noexcept;
    ~PSDA();
    PSDA& operator=(PSDA rhs);

    T& operator[](int i);
    const T& operator[](int i) const;
    int Size() const;
    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;


    void Integrate(int left, int right);
    void Differentiate(int left, int right);
    void LazyIncrement(int left, int right, const T& val);
    const T& Query(int left, int right) const;

    friend void swap<T>(PSDA<T>& lhs, PSDA<T>& rhs);
};

template <typename T>
void swap<T>(PSDA<T>& lhs, PSDA<T>& rhs)
{
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.size_, rhs.size_);
}



template <typename T>
PSDA<T>::PSDA(const T* arr, int size) : 
    data_(static_cast<T*>(operator new[](size * sizeof(T)))), size_(size)
{
    assert(size >= 0);

    //std::uninitialized copy
    for (int i = 0; i < size; ++i)
    {
        new (data_ + i) T(arr[i]);
    }
}

template <typename T>
PSDA<T>::PSDA(const T* begin, const T* end) : 
    PSDA(begin, end - begin)
{
    assert(begin <= end);
}

template <typename T>
PSDA<T>::PSDA(std::initializer_list<T> lst) : 
    PSDA(lst.begin(), static_cast<int>(lst.size()))
{
    assert(lst.size() >= 0);
}

template <typename T>
PSDA<T>::PSDA(const PSDA& other) : 
    PSDA(other.data_, other.size_)
{
    assert(other.size_ >= 0);
}

template <typename T>
PSDA<T>::PSDA(PSDA&& other) noexcept : 
    size_(std::exchange(other.size_, 0)), data_(std::exchange(other.data_, nullptr))
{
    assert(other.size_ >= 0);
}

template <typename T>
PSDA<T>::~PSDA()
{
    delete[] data_;
}

template <typename T>
PSDA<T>& PSDA<T>::operator=(PSDA rhs)
{
    swap(*this, rhs);
    return *this;
}

template <typename T>
T& PSDA<T>::operator[](int i)
{
    assert(0 <= i && i < size_);
    return data_[i];
}

template <typename T>
const T& PSDA<T>::operator[](int i) const
{
    assert(0 <= i && i < size_);
    return data_[i];
}


template <typename T>
int PSDA<T>::Size() const
{
    return size_;
}

template <typename T>
T* PSDA<T>::begin()
{
    return data_;
}

template <typename T>
const T* PSDA<T>::begin() const
{
    return data_;
}

template <typename T>
T* PSDA<T>::end()
{
    return data_ + size_;
}

template <typename T>
const T* PSDA<T>::end() const
{
    return data_ + size_;
}






template <typename T>
void PSDA<T>::Integrate(int left, int right)
{
    assert(0 <= left && left <= right && right < size_);

    for (int i = left + 1; i <= right; ++i)
    {
        data_[i] += data_[i - 1];
    }
}

template <typename T>
void PSDA<T>::Differentiate(int left, int right)
{
    assert(0 <= left && left <= right && right < size_);

    T prev = data_[left];

    for (int i = left + 1; i <= right; ++i)
    {
        data_[i] -= prev;
        prev += data_[i];
    }
}

template <typename T>
void PSDA<T>::LazyIncrement(int left, int right, const T& val)
{
    assert(0 <= left && left <= right && right < size_);

    data_[left] += val;
    if (right + 1 < size_) data_[right + 1] -= val;
}

template <typename T>
const T& PSDA<T>::Query(int left, int right) const
{
    assert(0 <= left && left <= right && right < size_);

    T total = data_[right];
    if (left - 1 >= 0) total -= data_[left - 1];

    return total;
}