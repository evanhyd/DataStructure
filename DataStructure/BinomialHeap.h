#pragma once
#include <vector>
#include <utility>
#include <cassert>

#ifdef _DEBUG
#include <iostream>
#include <string>
#endif

template <typename T, typename Predicate = std::greater<T>>
class BinomialHeap;

template <typename T, typename Predicate>
void swap(BinomialHeap<T, Predicate>& lhs, BinomialHeap<T, Predicate>& rhs);

template <typename T, typename Predicate>
class BinomialHeap {
  class FlagTree {
    T _data;
    FlagTree* _left;
    FlagTree* _right;

    template <typename ...Args>
    FlagTree(Args&&... args) : 
      _data(std::forward<Args>(args)...), _left(nullptr), _right(nullptr) {
    }

    FlagTree(const FlagTree& rhs) : 
      _data(rhs._data), _left((rhs._left ? new FlagTree(*rhs._left) : nullptr)), _right(_right = (rhs._right ? new FlagTree(*rhs._right) : nullptr)) {
    }

    FlagTree(FlagTree& rhs) :
      FlagTree(const_cast<const FlagTree&>(rhs)) {
      //override perfect forwarding from variadic constructor
    }

    FlagTree(FlagTree&& rhs) noexcept :
      _data(std::move(rhs._data)), _left(std::exchange(rhs._left, nullptr)), _right(std::exchange(rhs._right, nullptr)) {
    }

    ~FlagTree() {
      delete _left;
      delete _right;
    }

    FlagTree& operator=(const FlagTree& rhs) = delete;
    FlagTree& operator=(FlagTree&& rhs) = delete;

    friend class BinomialHeap;
  };

  std::vector<FlagTree*> _trees;
  std::size_t _size;

public:
  BinomialHeap() : 
    _trees(), _size(0) {
  }

  BinomialHeap(const BinomialHeap& rhs) :
    _trees(rhs._trees.size()), _size(rhs._size) {
    for (int h = 0; h < rhs._trees.size(); ++h) {
      if (rhs._trees[h]) {
        FlagTree* t = rhs._trees[h];
        _trees[h] = new FlagTree(*t);
      }
    }
  }

  BinomialHeap(BinomialHeap&& rhs) noexcept :
    _trees(std::move(rhs._trees)), _size(std::exchange(rhs._size, 0)) {
  }

  ~BinomialHeap() {
    for (const FlagTree* tree : _trees) {
      delete tree;
    }
  }

  BinomialHeap& operator=(BinomialHeap rhs) {
    swap(*this, rhs);
    return *this;
  }

  std::size_t Size() const {
    return _size;
  }

  bool Empty() const {
    return _size == 0;
  }

  const T& Top() const {
    return _trees[GetMaxIndex()]->_data;
  }

  T& Top() {
    return _trees[GetMaxIndex()]->_data;
  }

  template <typename... Args>
  void Push(Args&&... args) {
    MergeToHeight(new FlagTree(std::forward<Args>(args)...), 0);
    ++_size;
  }

  void Merge(BinomialHeap heap) {
    for (int h = 0; h < heap._trees.size(); ++h) {
      if (FlagTree* tree = heap._trees[h]; tree) {
        MergeToHeight(tree, h);
      }
    }
    _size += heap._size;

    heap._trees.clear();
    heap._size = 0;
  }

  void Pop() {
    int h = GetMaxIndex();
    FlagTree* tree = _trees[h]->_left;
    _trees[h]->_left = nullptr;
    delete _trees[h];
    _trees[h] = nullptr;

    while (tree) {
      --h;
      FlagTree* const next = tree->_right;
      tree->_right = nullptr;
      MergeToHeight(tree, h);
      tree = next;
    }

    --_size;
  }

  void Debug() const {
    #ifdef _DEBUG 
    std::cout << "size: " << _size << '\n';
    for (const FlagTree* tree : _trees) {
      Print("", tree, false);
    }
    #endif
  }

private:
  void MergeToHeight(FlagTree* tree0, int height) {
    const Predicate cmp{};
    while (true) {
      if (height >= _trees.size()) {
        _trees.resize(height + 1);
        break;
      }

      if (FlagTree* tree1 = _trees[height]; tree1) {
        if (cmp(tree1->_data, tree0->_data)) {
          std::swap(tree0, tree1);
        }
        tree1->_right = tree0->_left;
        tree0->_left = tree1;
        _trees[height++] = nullptr;
      } else {
        break;
      }
    }

    _trees[height] = tree0;
  }

  int GetMaxIndex() const {
    assert(_size != 0);
    const Predicate cmp{};
    int m = -1;
    for (int h = 0; h < _trees.size(); ++h) {
      if (_trees[h] && (m == -1 || cmp(_trees[h]->_data, _trees[m]->_data))) {
        m = h;
      }
    }
    return m;
  }

  #ifdef _DEBUG
  void Print(const std::string& prefix, const FlagTree* node, bool isRight) const {
    if (node) {
      std::cout << prefix << (isRight ? "|--" : "L--") << node->_data << '\n';
      Print(prefix + (isRight ? "    " : "|   "), node->_right, true);
      Print(prefix + (isRight ? "    " : "|   "), node->_left, false);
    }
  }
  #endif

  friend void swap<T, Predicate>(BinomialHeap& lhs, BinomialHeap& rhs);
};

template <typename T, typename Predicate>
void swap(BinomialHeap<T, Predicate>& lhs, BinomialHeap<T, Predicate>& rhs) {
  using std::swap;
  swap(lhs._trees, rhs._trees);
  swap(lhs._size, rhs._size);
}