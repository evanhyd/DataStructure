#pragma once
#include <vector>

template <typename T, typename Predicate = std::less<T>>
class BinaryHeap
{
    std::vector<T> data_;

public:
    BinaryHeap() = default;
    BinaryHeap(std::vector<T> arr);

    bool IsEmpty() const;
    void Clear();
    void Push(const T& val);
    void Pop(Predicate p = Predicate());
    const T& Top() const;

private:
    void HeapSort(int r, Predicate p = Predicate());
};


template <typename T, typename Predicate>
BinaryHeap<T, Predicate>::BinaryHeap(std::vector<T> arr) : data_(std::move(arr))
{
    for (int r = 0; r < data_.size(); ++r)
    {
        this->HeapSort(r);
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

    int r = 0;
    while (r < data_.size())
    {
        int left = r * 2 + 1;
        int right = left + 1;
        int next = r;

        //compare to the two child nodes
        if (left < data_.size() && p(data_[left], data_[next])) next = left;
        if (right < data_.size() && p(data_[right], data_[next])) next = right;

        //no larger element to swap
        if (next == r) break;

        std::swap(data_[r], data_[next]);
        r = next;
    }
}

template <typename T, typename Predicate>
const T& BinaryHeap<T, Predicate>::Top() const
{
    return data_.front();
}



template <typename T, typename Predicate>
void BinaryHeap<T, Predicate>::HeapSort(int r, Predicate p)
{
    int parent = (r - 1) / 2;

    while (r && p(data_[r], data_[parent]))
    {
        std::swap(data_[r], data_[parent]);
        r = parent;
        parent = (r - 1) / 2;
    }
}