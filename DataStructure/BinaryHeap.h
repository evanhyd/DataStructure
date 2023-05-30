#pragma once
#include <vector>

template <typename T, typename Predicate = std::greater<T>>
class BinaryHeap {
  std::vector<T> data_;

 public:
  BinaryHeap() = default;

  BinaryHeap(std::vector<T> arr) : data_(std::move(arr)) {
    for (int r = Parent(data_.size() - 1); r >= 0; --r) {
      FixDown(r);
    }
  }

  template <typename ...Args>
  void Push(Args&&... args) {
    data_.emplace_back(std::forward<Args>(args)...);
    FixUp(data_.size() - 1);
  }

  void Pop() {
    data_.front() = std::move(data_.back());
    data_.pop_back();
    FixDown(0);
  }

  const T& Top() const { 
    assert(!data_.empty() && "access empty heap top");
    return data_[0];
  }

  size_t Size() const { 
    return data_.size();
  }

 private:
  int Parent(int n) const { return (n - 1) / 2; }
  int LeftChild(int n) const { return n * 2 + 1; }
  int RightChild(int n) const { return n * 2 + 2; }

   void FixUp(int n, Predicate p = Predicate()) { 
     while (n > 0) {
      int parent = Parent(n);
      if (p(data_[n], data_[parent])) {
        std::swap(data_[n], data_[parent]);
        n = parent;
      } else {
        break;
      }
     }
   }

   void FixDown(int n, Predicate p = Predicate()) {
     while (true) {
      int child = LeftChild(n);
      if (child >= data_.size()) {
        break;
      }
      if (int rightChild = RightChild(n);
          rightChild < data_.size() && p(data_[rightChild], data_[child])) {
        child = rightChild;
      }
      if (p(data_[child], data_[n])) {
        std::swap(data_[n], data_[child]);
        n = child;
      } else {
        break;
      }
     }
   }
};