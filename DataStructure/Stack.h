#pragma once
#include <algorithm>

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


    void Push(const T& item);
    void Pop();
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
    using namespace std;
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
void Stack<T>::Push(const T& item)
{
    if (size_ == capacity_) this->Reserve(capacity_ * 2 + 1);

    //push into the stack
    data_[size_++] = item;
}

template <typename T>
void Stack<T>::Pop()
{
    --size_;
}

template <typename T>
void Stack<T>::Clear()
{
    size_ = 0;
}

template <typename T>
void Stack<T>::Reserve(int new_capacity)
{
    //expand the capacity
    //+1 to avoid edge case when the capacity is 0
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
    //shrink to current size
    capacity_ = size_;

    //allocate the new resource first
    T* new_data = new T[capacity_];

    //update the internal data
    std::copy(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
}



template <typename T>
T& Stack<T>::Front()
{
    return data_[0];
}
template <typename T>
const T& Stack<T>::Front() const
{
    return data_[0];
}
template <typename T>
T& Stack<T>::Back()
{
    return data_[size_-1];
}
template <typename T>
const T& Stack<T>::Back() const
{
    return data_[size_-1];
}


template <typename T>
T& Stack<T>::operator[](int i)
{
    return data_[i];
}
template <typename T>
const T& Stack<T>::operator[](int i) const
{
    return data_[i];
}

