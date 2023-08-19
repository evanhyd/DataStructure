#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <random>
#include <cassert>

template <typename KeyType, typename ValueType, typename Predicate = std::less<KeyType>>
class SkipList {
  using KeyValueType = std::pair<const KeyType, ValueType>;

  static inline const Predicate comp{};

  struct Tower;

  struct Node {
    Tower* _next;
    Node() : _next(nullptr) {}
  };

  struct Tower {
    std::optional<KeyValueType> _keyValue;
    std::vector<Node> _nodes; //height = size - 1, first node owns the tower

    Tower() : _keyValue(), _nodes(1) {}

    Tower(const KeyValueType& keyValue) : _keyValue(keyValue) {
      static std::default_random_engine engine(unsigned(std::chrono::system_clock().now().time_since_epoch().count()));
      static std::bernoulli_distribution dist(1.0 / 2.71828182846); // 1/e
      int size = 1;
      while (dist(engine)) {
        ++size;
      }
      _nodes.resize(size);
    }

    const KeyType& Key() {
      return _keyValue->first;
    }

    ValueType& Value() {
      return _keyValue->second;
    }
  };

  template <typename ReturnType>
  class IteratorImpl {
    Tower* tower;
    IteratorImpl(Tower* tower) : tower(tower) {}

  public:
    IteratorImpl& operator++() {
      assert(tower->_nodes.at(0) && "incrementing pass the end");
      tower = tower->_nodes[0];
      return *this;
    }

    bool operator==(const IteratorImpl& other) const {
      return tower == other.tower;
    }

    bool operator!=(const IteratorImpl& other) const {
      return !(*this == other);
    }

    ReturnType operator*() {
      assert(tower && tower->_keyValue && "dereference sentinel node");
      return *tower->_keyValue;
    }
    friend class SkipList;
  };

  Tower* _root;

  std::vector<Node*> GetPredecessors(const KeyType& key) const {
    Tower* currTower = _root;
    int height = int(_root->_nodes.size()) - 1;

    std::vector<Node*> predecessors = { &currTower->_nodes[height] };

    //go down by 1 height each iteration
    while (--height >= 0) {

      //search horizontally until reaches the upper bound or sentinel
      Node* currNode = &currTower->_nodes[height];
      while (currNode->_next && comp(currNode->_next->Key(), key)) {
        currNode = &currNode->_next->_nodes[height];
      }
      predecessors.push_back(currNode);
    }
    return predecessors;
  }

public:
  using Iterator = IteratorImpl<KeyValueType&>;
  using ConstIterator = IteratorImpl<const KeyValueType&>;

  Iterator begin() {
    return Iterator(_root->_nodes.front()->_next);
  }

  ConstIterator begin() const {
    return ConstIterator(_root->_nodes.front()->_next);
  }

  Iterator end() {
    return Iterator(nullptr);
  }

  ConstIterator end() const {
    return ConstIterator(nullptr);
  }

  Iterator Find(const KeyType& key) {
    Tower* const tower = GetPredecessors(key).back()->_next;
    return Iterator((tower && !comp(tower->Key(), key) && !comp(key, tower->Key())) ? tower : nullptr); //is == defined?, should I use < instead?
  }

  ConstIterator Find(const KeyType& key) const {
    Tower* const tower = GetPredecessors(key).back()->_next;
    return ConstIterator((tower && !comp(tower->Key(), key) && !comp(key, tower->Key())) ? tower : nullptr);
  }

  void Insert(const KeyValueType& keyValue) {
    if (auto result = Find(keyValue.first); result != end()) {
      (*result).second = keyValue.second;
      return;
    }

    //insert the new tower
    Tower* newTower = new Tower(keyValue); //pray it doesn't throw, this strategy always works
    if (_root->_nodes.size() <= newTower->_nodes.size()) {
      _root->_nodes.resize(newTower->_nodes.size() + 1);
    }
    auto predecessors = GetPredecessors(keyValue.first);

    for (Node& node : newTower->_nodes) {
      node._next = predecessors.back()->_next;
      predecessors.back()->_next = newTower;
      predecessors.pop_back();
    }
  }

  void Erase(const KeyType& key) {
    auto predecessors = GetPredecessors(key);
    Tower* const tower = predecessors.back()->_next;
    bool found = false;

    int height = 0;
    for (auto iter = predecessors.rbegin(); iter != predecessors.rend(); ++iter, ++height) {
      Tower* const candidate = (*iter)->_next;
      if (candidate && !comp(candidate->Key(), key) && !comp(key, candidate->Key())) {
        (*iter)->_next = candidate->_nodes[height]._next;
        found = true;
      } else {
        break;
      }
    }

    if (found) {
      delete tower;
    }
  }

  void Print() const {
    for (int i = int(_root->_nodes.size()) - 1; i >= 0; --i) {
      Node node = _root->_nodes[i];
      for (Tower* tower = node._next; tower; ) {
        std::cout << "_(" << tower->Key() << ", " << tower->Value() << ")";
        tower = tower->_nodes[i]._next;
      }
      std::cout << '\n';
    }
  }

  explicit SkipList() : _root(new Tower()) {}

  explicit SkipList(const SkipList& other) = delete;

  explicit SkipList(SkipList&& other) noexcept : _root(std::exchange(other._root, nullptr)) {}

  ~SkipList() {
    for (Tower* curr = _root; curr; ) {
      Tower* next = curr->_nodes.front()._next;
      delete curr;
      curr = next;
    }
  }

  SkipList& operator=(const SkipList& other) = delete;

  SkipList& operator=(SkipList&& other) noexcept {
    std::swap(_root, other._root);
  }
};
