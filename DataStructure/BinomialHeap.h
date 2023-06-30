#pragma once
#include <vector>
#include <utility>

#ifdef _DEBUG
#include <iostream>
#include <string>
#endif

template <typename T>
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

    FlagTree(FlagTree&& rhs) noexcept :
      _data(std::move(rhs._data)), _left(std::exchange(rhs._left, nullptr)), _right(std::exchange(rhs._right, nullptr)) {
    }

    FlagTree& operator=(const FlagTree& rhs) = delete;
    FlagTree& operator=(FlagTree&& rhs) = delete;

    ~FlagTree() {
      delete _left;
      delete _right;
    }

    friend class BinomialHeap;
  };

  std::vector<FlagTree*> _trees;

public:
  template <typename... Args>
  void Push(Args&&... args) {
    FlagTree* t0 = new FlagTree(std::forward<Args>(args)...);
    int height = 0;
    while (true) {
      if (height >= _trees.size()) {
        _trees.push_back(nullptr);
        break;
      }

      if (!_trees[height]) {
        break;
      }

      FlagTree* t1 = _trees[height];
      if (t0->_data < t1->_data) {
        std::swap(t0, t1);
      }
      t1->_right = t0->_left;
      t0->_left = t1;

      _trees[height] = nullptr;
      ++height;
    }
    _trees[height] = t0;
  }

  void Debug() const {
    #ifdef _DEBUG 
    std::cout << "size: " << _trees.size() << '\n';
    for (const FlagTree* tree : _trees) {
      Print("", tree, false);
    }
    #endif
  }

private:
  #ifdef _DEBUG
  void Print(const std::string& prefix, const FlagTree* node, bool isRight) const {
    if (node) {
      std::cout << prefix << (isRight ? "|--" : "L--") << node->_data << '\n';
      Print(prefix + (isRight ? "    " : "|   "), node->_right, true);
      Print(prefix + (isRight ? "    " : "|   "), node->_left, false);
    }
  }
  #endif
};

