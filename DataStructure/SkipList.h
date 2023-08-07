#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <random>
#include <cassert>

template <typename KeyType, typename ValueType, typename Predicate = std::less<KeyType>>
class SkipList {

  struct Tower;

  struct Node {
    Tower* _next;
    Node() : _next(nullptr) {}
  };

  struct Tower {
    const std::unique_ptr<KeyType> _key;
    std::unique_ptr<ValueType> _value;
    std::vector<Node> _nodes; //height = size - 1, first node owns the tower

    Tower() : _key(), _value(), _nodes(1) {}

    Tower(const std::pair<KeyType, ValueType>& keyValue)
      : _key(std::make_unique<KeyType>(keyValue.first)), _value(std::make_unique<ValueType>(keyValue.second)) {
      static std::default_random_engine engine(unsigned(std::chrono::system_clock().now().time_since_epoch().count()));
      static std::bernoulli_distribution dist(1.0 / 2.71828182846); // 1/e
      int size = 1;
      while (dist(engine)) {
        ++size;
      }
      _nodes.resize(size);
    }
  };

  template <typename ReturnValueType>
  class IteratorImpl {
    Tower* tower;

    IteratorImpl(Tower* tower) : tower(tower) {}

  public:

    IteratorImpl& operator++() {
      assert((*tower)[0] && "incrementing pass the end");
      tower = (*tower)[0];
      return *this;
    }

    bool operator==(const IteratorImpl& other) const {
      return tower == other.tower;
    }

    bool operator!=(const IteratorImpl& other) const {
      return !(*this == other);
    }

    std::pair<const KeyType&, ReturnValueType&> operator*() {
      assert(tower && "dereference sentinel node");
      return { *tower->_key, *tower->_value };
    }
    friend class SkipList;
  };

private:
  Tower* _root;

  std::vector<Node*> GetPredecessors(const KeyType& key) const {
    const Predicate pre{};

    Tower* currTower = _root;
    int height = int(_root->_nodes.size()) - 1;

    std::vector<Node*> predecessors = { &currTower->_nodes[height] };

    //go down by 1 height each iteration
    while (--height >= 0) {

      //search horizontally until reaches the upper bound or sentinel
      Node* currNode = &currTower->_nodes[height];
      while (currNode->_next && pre(*currNode->_next->_key, key)) {
        currNode = &currNode->_next->_nodes[height];
      }
      predecessors.push_back(currNode);
    }
    return predecessors;
  }

public:
  using Iterator = IteratorImpl<ValueType>;
  using ConstIterator = IteratorImpl<const ValueType>;

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
    return Iterator((tower && *tower->_key == key) ? tower : nullptr);
  }

  ConstIterator Find(const KeyType& key) const {
    Tower* const tower = GetPredecessors(key).back()->_next;
    return ConstIterator((tower && *tower->_key == key) ? tower : nullptr);
  }

  void Insert(const std::pair<KeyType, ValueType>& keyValue) {
    auto predecessors = GetPredecessors(keyValue.first);

    //check if already exists
    if (Tower* const tower = predecessors.back()->_next; tower && *tower->_key == keyValue.first) {
      *tower->_value = keyValue.second;
      return;
    }

    //insert the new tower
    Tower* newTower = new Tower(keyValue); //pray it doesn't throw, this strategy always works
    if (newTower->_nodes.size() >= _root->_nodes.size()) {
      _root->_nodes.resize(newTower->_nodes.size() + 1);
    }
    predecessors = GetPredecessors(keyValue.first);

    for (Node& node : newTower->_nodes) {
      node._next = predecessors.back()->_next;
      predecessors.back()->_next = newTower;
      predecessors.pop_back();
    }
  }

  void Print() const {
    for (int i = int(_root->_nodes.size()) - 1; i >= 0; --i) {
      Node node = _root->_nodes[i];
      for (Tower* tower = node._next; tower; ) {
        std::cout << "_(" << *tower->_key << ", " << *tower->_value << ")";
        tower = tower->_nodes[i]._next;
      }
      std::cout << '\n';
    }
  }

  SkipList() : _root(new Tower()) {

  }

  ~SkipList() {
    for (Tower* curr = _root; curr; ) {
      Tower* next = curr->_nodes.front()._next;
      delete curr;
      curr = next;
    }
  }
};
