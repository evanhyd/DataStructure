#pragma once
#include <vector>

template<typename T>
class DisjointSet
{
    struct Set
    {
        int pos;
        T data;
    };

    std::vector<Set> sets_;

public:
    DisjointSet(int size, const T& value = T{});

    T& operator[](int pos);
    const T& operator[](int pos) const;

    int Size() const;

    int Find(int pos);
    void Union(int root, int child);
    bool InSameSet(int s0, int s1);
};


template<typename T>
DisjointSet<T>::DisjointSet(int size, const T& value) : sets_(size)
{
    for (int i = 0; i < sets_.size(); ++i)
    {
        sets_[i].pos = i;
        sets_[i].data = value;
    }
}

template <typename T>
T& DisjointSet<T>::operator[](int pos)
{
    assert(0 <= pos && pos < sets_.size());
    return sets_[pos].data;
}

template <typename T>
const T& DisjointSet<T>::operator[](int pos) const
{
    assert(0 <= pos && pos < sets_.size());
    return sets_[pos].data;
}

template <typename T>
int DisjointSet<T>::Size() const
{
    return static_cast<int>(sets_.size());
}




template<typename T>
int DisjointSet<T>::Find(int pos)
{
    assert(0 <= pos && pos < sets_.size());

    std::vector<Set*> stack;
    stack.push_back(&sets_[pos]);

    while (true)
    {
        const Set& top = *stack.back();

        //terminate when set's position == index
        if (top.pos == sets_[top.pos].pos) break;
        stack.push_back(&sets_[top.pos]);
    }

    for (int i = 0; i < stack.size() - 1; ++i)
    {
        stack[i]->pos = stack.back()->pos;
    }

    return stack.back()->pos;
}

template <typename T>
void DisjointSet<T>::Union(int root, int child)
{
    assert(0 <= root && root < sets_.size());
    assert(0 <= child && child < sets_.size());

    int root_pos = Find(root);
    int child_pos = Find(child);
    sets_[child_pos].pos = root_pos;
}

template <typename T>
bool DisjointSet<T>::InSameSet(int s0, int s1)
{
    assert(0 <= s0 && s0 < sets_.size());
    assert(0 <= s1 && s1 < sets_.size());

    return Find(s0) == Find(s1);
}
