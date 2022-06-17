#pragma once
#include <vector>
#include "io.h"

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
    DisjointSet() = default;
    DisjointSet(int size, const T& val = T{});

    T& operator[](int setID);
    const T& operator[](int setID) const;

    constexpr int Size() const;
    constexpr bool Empty() const;

    int Find(int setID);
    void Union(int set0, int set1);
    bool InSameSet(int set0, int set1);


    void PushBack(const T& val);
    void PushBack(T&& val);
    void Resize(int new_size, const T& val = T{});
    void Reserve(int new_reserved);
};


template<typename T>
DisjointSet<T>::DisjointSet(int size, const T& val) : sets_(size)
{
    for (int r = 0; r < sets_.size(); ++r)
    {
        sets_[r].parent = r;
        sets_[r].data = val;
    }
}

template <typename T>
T& DisjointSet<T>::operator[](int setID)
{
    assert(0 <= setID && setID < sets_.size());
    return sets_[setID].data;
}

template <typename T>
const T& DisjointSet<T>::operator[](int setID) const
{
    assert(0 <= setID && setID < sets_.size());
    return sets_[setID].data;
}

template <typename T>
constexpr int DisjointSet<T>::Size() const
{
    return static_cast<int>(sets_.size());
}

template <typename T>
constexpr bool DisjointSet<T>::Empty() const
{
    return sets_.empty();
}




template<typename T>
int DisjointSet<T>::Find(int setID)
{
    assert(0 <= setID && setID < sets_.size());

    std::vector<Set*> stack = { &sets_[setID] };
    while (true)
    {
        const Set& top = *stack.back();
        cug::io::Log("{}\n", &top);
        if (top.parent == setID) break;
        setID = top.parent;
        stack.push_back(&sets_[top.parent]);
        cug::io::Log("{}\n", &top);
    }

    for (int r = 0; r < stack.size() - 1; ++r)
    {
        stack[r]->parent = setID;
    }

    return setID;
}

template <typename T>
void DisjointSet<T>::Union(int set0, int set1)
{
    assert(0 <= set0 && set0 < sets_.size());
    assert(0 <= set1 && set1 < sets_.size());

    int root0 = Find(set0);
    int root1 = Find(set1);

    if (root0 == root1) return;

    //add () since gcc believes rank is a template, wtf???
    if ((sets_[root0].rank) < (sets_[root1].rank)) sets_[root0].parent = root1;
    else if ((sets_[root0].rank) > (sets_[root1].rank)) sets_[root1].parent = root0;
    else 
    {
        sets_[root1].parent = root0;
        ++sets_[root0].rank;
    }
}

template <typename T>
bool DisjointSet<T>::InSameSet(int set0, int set1)
{
    return Find(set0) == Find(set1);
}

template <typename T>
void DisjointSet<T>::PushBack(const T& val)
{
    sets_.emplace_back(static_cast<int>(sets_.size()), 0, val);
}

template <typename T>
void DisjointSet<T>::PushBack(T&& val)
{
    sets_.emplace_back(static_cast<int>(sets_.size()), 0, std::move(val));
}

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