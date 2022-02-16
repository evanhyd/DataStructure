#pragma once
#include <algorithm>
#include <cassert>
#include "debug.h"

#define DEBUG_MODE true

template <typename T> class Stack;
template <typename T> void swap(Stack<T>& s1, Stack<T>& s2);

template <typename T>
class Stack
{
    T* data_;
    int capacity_;
    int size_;

public:

    Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    ~Stack();

    Stack& operator=(Stack s);

    bool Empty() const;
    int Size() const;
    int Capacity() const;


    void PushBack(const T& item);
    void PushBack(T&& item);
    void PopBack();
    void Clear();
    void Reserve(int new_capacity);
    void ShrinkToFit();


    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;

    T& operator[](int i);
    const T& operator[](int i) const;

    friend void swap<T>(Stack<T>& s1, Stack<T>& s2);
};


template <typename T>
void swap(Stack<T>& s1, Stack<T>& s2)
{
    using std::swap;
    swap(s1.data_, s2.data_);
    swap(s1.capacity_, s2.capacity_);
    swap(s1.size_, s2.size_);
}


// constructors
template <typename T>
Stack<T>::Stack() : data_(nullptr), capacity_(0), size_(0)
{
    //empty
}

template <typename T>
Stack<T>::Stack(const Stack& other) : data_(new T[other.capacity_]), capacity_(other.capacity_), size_(other.size_)
{
    //copy over by values
    std::copy(other.data_, other.data_ + other.size_, data_);
}

template <typename T>
Stack<T>::Stack(Stack&& other) noexcept : Stack()
{
    swap(*this, other);
}


//destructor
template <typename T>
Stack<T>::~Stack()
{
    delete[] data_;
}


//assignment operators copy + move
template <typename T>
Stack<T>& Stack<T>::operator=(Stack s)
{
    swap(*this, s);
    return *this;
}




template <typename T>
bool Stack<T>::Empty() const
{
    return size_ == 0;
}

template <typename T>
int Stack<T>::Size() const
{
    return size_;
}

template <typename T>
int Stack<T>::Capacity() const
{
    return capacity_;
}





template <typename T>
void Stack<T>::PushBack(const T& item)
{
    if (size_ == capacity_) this->Reserve(capacity_ * 2 + 1);
    data_[size_++] = item;
}

template <typename T>
void Stack<T>::PushBack(T&& item)
{
    if (size_ == capacity_) this->Reserve(capacity_ * 2 + 1);
    data_[size_++] = std::move(item);
}

template <typename T>
void Stack<T>::PopBack()
{
    assert(!this->Empty());
    --size_;
    //data_[--size_].~T();
}

template <typename T>
void Stack<T>::Clear()
{
    size_ = 0;
}

template <typename T>
void Stack<T>::Reserve(int new_capacity)
{
    if (new_capacity < size_) return;

    //adjust the capacity
    capacity_ = new_capacity;

    //allocate the new resource first
    T* new_data = new T[capacity_];

    //update the internal data
    std::copy(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
}

template <typename T>
void Stack<T>::ShrinkToFit()
{
    this->Reserve(size_);
}



template <typename T>
T& Stack<T>::Front()
{
    assert(!this->Empty());
    return data_[0];
}
template <typename T>
const T& Stack<T>::Front() const
{
    assert(!this->Empty());
    return data_[0];
}
template <typename T>
T& Stack<T>::Back()
{
    assert(!this->Empty());
    return data_[size_-1];
}
template <typename T>
const T& Stack<T>::Back() const
{
    assert(!this->Empty());
    return data_[size_-1];
}


template <typename T>
T& Stack<T>::operator[](int i)
{
    assert(0 <= i && i < size_);
    return data_[i];
}
template <typename T>
const T& Stack<T>::operator[](int i) const
{
    assert(0 <= i && i < size_);
    return data_[i];
}

