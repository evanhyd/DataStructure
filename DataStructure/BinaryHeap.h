#pragma once
#include <vector>

template <typename T, typename Predicate = std::greater<T>>
class BinaryHeap {
  std::vector<T> _data;

 public:
  BinaryHeap() = default;

  BinaryHeap(std::vector<T> arr) : _data(std::move(arr)) {
    for (int r = Parent(_data.size() - 1); r >= 0; --r) {
      FixDown(r);
    }
  }

  template <typename ...Args>
  void Push(Args&&... args) {
    _data.emplace_back(std::forward<Args>(args)...);
    FixUp(_data.size() - 1);
  }

  void Pop() {
    _data.front() = std::move(_data.back());
    _data.pop_back();
    FixDown(0);
  }

  const T& Top() const { 
    assert(!_data.empty() && "access empty heap top");
    return _data[0];
  }

  size_t Size() const { 
    return _data.size();
  }

 private:
  int Parent(int n) const { return (n - 1) / 2; }
  int LeftChild(int n) const { return n * 2 + 1; }
  int RightChild(int n) const { return n * 2 + 2; }

   void FixUp(int n, Predicate p = Predicate()) { 
     while (n > 0) {
      int parent = Parent(n);
      if (p(_data[n], _data[parent])) {
        std::swap(_data[n], _data[parent]);
        n = parent;
      } else {
        break;
      }
     }
   }

   void FixDown(int n, Predicate p = Predicate()) {
     while (true) {
      int child = LeftChild(n);
      if (child >= _data.size()) {
        break;
      }
      if (int rightChild = RightChild(n);
          rightChild < _data.size() && p(_data[rightChild], _data[child])) {
        child = rightChild;
      }
      if (p(_data[child], _data[n])) {
        std::swap(_data[n], _data[child]);
        n = child;
      } else {
        break;
      }
     }
   }
};