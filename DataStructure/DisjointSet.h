#pragma once
#include <vector>

template<typename T>
class DisjointSet
{
    struct Set
    {
        int parent;
        int rank;
        T data;
    };

    std::vector<Set> sets_;

public:
    bool InSameSet(int set0, int set1);

    void PushBack(const T& val);
    void PushBack(T&& val);
    void Resize(int new_size, const T& val = T{});
    void Reserve(int new_reserved);
};

template <typename T>
void DisjointSet<T>::Resize(int new_size, const T& val)
{
    sets_.resize(new_size, val);
}

template <typename T>
void DisjointSet<T>::Reserve(int new_reserved)
{
    sets_.reserve(new_reserved);
}