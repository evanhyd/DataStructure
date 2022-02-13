#pragma once
#include <vector>

template <typename T, typename Predicate = std::less<T>>
class BinaryHeap
{
    std::vector<T> data_;

public:
    BinaryHeap();
    BinaryHeap(const std::vector<T>& arr);

    bool IsEmpty() const;
    void Clear();
    void Push(const T& val);
    void Pop(Predicate p = Predicate());
    const T& Top() const;

private:
    void HeapSort(int i, Predicate p = Predicate());
};


template <typename T, typename Predicate>
BinaryHeap<T, Predicate>::BinaryHeap() : data_()
{
    //empty
}

template <typename T, typename Predicate>
BinaryHeap<T, Predicate>::BinaryHeap(const std::vector<T>& arr) : data_(arr)
{
    for (int i = 0; i < data_.size(); ++i)
    {
        this->HeapSort(i);
    }
}

template <typename T, typename Predicate>
bool BinaryHeap<T, Predicate>::IsEmpty() const
{
    return data_.empty();
}

template <typename T, typename Predicate>
void BinaryHeap<T, Predicate>::Clear()
{
    data_.clear();
}

template <typename T, typename Predicate>
void BinaryHeap<T, Predicate>::Push(const T& val)
{
    data_.push_back(val);
    this->HeapSort(int(data_.size()) - 1);
}

template <typename T, typename Predicate>
void BinaryHeap<T, Predicate>::Pop(Predicate p)
{
    data_.front() = data_.back();
    data_.pop_back();

    int i = 0;
    while (i < data_.size())
    {
        int left = i * 2 + 1;
        int right = left + 1;
        int next = i;

        //compare to the two child nodes
        if (left < data_.size() && p(data_[left], data_[next])) next = left;
        if (right < data_.size() && p(data_[right], data_[next])) next = right;

        //no larger element to swap
        if (next == i) break;

        std::swap(data_[i], data_[next]);
        i = next;
    }
}

template <typename T, typename Predicate>
const T& BinaryHeap<T, Predicate>::Top() const
{
    return data_.front();
}



template <typename T, typename Predicate>
void BinaryHeap<T, Predicate>::HeapSort(int i, Predicate p)
{
    int parent = (i - 1) / 2;

    while (i && p(data_[i], data_[parent]))
    {
        std::swap(data_[i], data_[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}