#pragma once
#include <iostream>
#include <vector>
#include <cassert>


//prefix sum difference array
template <typename T>
class PSDA
{
    std::vector<T> data_;

public:

    PSDA(std::vector<T> arr);

    int Size() const;
    T& operator[](int i);
    const T& operator[](int i) const;

    auto begin();
    const auto begin() const;
    auto end();
    const auto end() const;

    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;

    void Integrate(int left, int right);
    void Differentiate(int left, int right);
    void LazyIncrement(int left, int right, const T& val);
    const T& Query(int left, int right) const;
};


template <typename T>
PSDA<T>::PSDA(std::vector<T> arr) : data_(std::move(arr))
{
    //empty
}



template <typename T>
int PSDA<T>::Size() const
{
    return data_.size();
}

template <typename T>
T& PSDA<T>::operator[](int i)
{
    assert(0 <= i && i < data_.size());
    return data_[i];
}

template <typename T>
const T& PSDA<T>::operator[](int i) const
{
    assert(0 <= i && i < data_.size());
    return data_[i];
}

template <typename T>
auto PSDA<T>::begin()
{
    return data_.begin();
}

template <typename T>
const auto PSDA<T>::begin() const
{
    return data_.cbegin();
}

template <typename T>
auto PSDA<T>::end()
{
    return data_.end();
}

template <typename T>
const auto PSDA<T>::end() const
{
    return data_.cend();
}



template <typename T>
T& PSDA<T>::Front()
{
    return data_.front();
}

template <typename T>
const T& PSDA<T>::Front() const
{
    return data_.front();
}

template <typename T>
T& PSDA<T>::Back()
{
    return data_.back();
}

template <typename T>
const T& PSDA<T>::Back() const
{
    return data_.back();
}






template <typename T>
void PSDA<T>::Integrate(int left, int right)
{
    assert(0 <= left && left <= right && right < data_.size());

    for (int i = left + 1; i <= right; ++i)
    {
        data_[i] += data_[i - 1];
    }
}

template <typename T>
void PSDA<T>::Differentiate(int left, int right)
{
    assert(0 <= left && left <= right && right < data_.size());

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
    assert(0 <= left && left <= right && right < data_.size());

    data_[left] += val;
    if (right + 1 < data_.size()) data_[right + 1] -= val;
}

template <typename T>
const T& PSDA<T>::Query(int left, int right) const
{
    assert(0 <= left && left <= right && right < data_.size());

    T total = data_[right];
    if (left - 1 >= 0) total -= data_[left - 1];

    return total;
}