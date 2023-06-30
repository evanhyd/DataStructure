#pragma once
#include <iostream>
#include <vector>
#include <cassert>


//prefix sum difference array
template <typename T>
class PSDA
{
    std::vector<T> _data;

public:

    PSDA(std::vector<T> arr);

    int Size() const;
    T& operator[](int r);
    const T& operator[](int r) const;

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
PSDA<T>::PSDA(std::vector<T> arr) : _data(std::move(arr))
{
    //empty
}



template <typename T>
int PSDA<T>::Size() const
{
    return _data.size();
}

template <typename T>
T& PSDA<T>::operator[](int r)
{
    assert(0 <= r && r < _data.size());
    return _data[r];
}

template <typename T>
const T& PSDA<T>::operator[](int r) const
{
    assert(0 <= r && r < _data.size());
    return _data[r];
}

template <typename T>
auto PSDA<T>::begin()
{
    return _data.begin();
}

template <typename T>
const auto PSDA<T>::begin() const
{
    return _data.cbegin();
}

template <typename T>
auto PSDA<T>::end()
{
    return _data.end();
}

template <typename T>
const auto PSDA<T>::end() const
{
    return _data.cend();
}



template <typename T>
T& PSDA<T>::Front()
{
    return _data.front();
}

template <typename T>
const T& PSDA<T>::Front() const
{
    return _data.front();
}

template <typename T>
T& PSDA<T>::Back()
{
    return _data.back();
}

template <typename T>
const T& PSDA<T>::Back() const
{
    return _data.back();
}






template <typename T>
void PSDA<T>::Integrate(int left, int right)
{
    assert(0 <= left && left <= right && right < _data.size());

    for (int r = left + 1; r <= right; ++r)
    {
        _data[r] += _data[r - 1];
    }
}

template <typename T>
void PSDA<T>::Differentiate(int left, int right)
{
    assert(0 <= left && left <= right && right < _data.size());

    T prev = _data[left];

    for (int r = left + 1; r <= right; ++r)
    {
        _data[r] -= prev;
        prev += _data[r];
    }
}

template <typename T>
void PSDA<T>::LazyIncrement(int left, int right, const T& val)
{
    assert(0 <= left && left <= right && right < _data.size());

    _data[left] += val;
    if (right + 1 < _data.size()) _data[right + 1] -= val;
}

template <typename T>
const T& PSDA<T>::Query(int left, int right) const
{
    assert(0 <= left && left <= right && right < _data.size());

    T total = _data[right];
    if (left - 1 >= 0) total -= _data[left - 1];

    return total;
}