#pragma once
#include <vector>
#include <utility>
#include <cassert>

template <typename Key, typename Value, typename Predicate = std::less<Key>>
class SkipList {
  class Node {
    Key _key;
    Node* _next;
    Node* _below;
    Node(const Key& key) : _key(key) {}
    friend class SkipList;
  };

  class ValueNode : Node {
    Value _value;
    ValueNode(const Key& key, const Value value) : Node(key), _value(value) {}
    friend class SkipList;
  };

  template <typename ReturnType>
  class IteratorImpl {
    ValueNode* _node;
    IteratorImpl(ValueNode* node) : _node(node) {}

  public:
    IteratorImpl& operator++() {
      assert(_node && "increment pass the end iterator");
      _node = _node->_next;
    }

    bool operator==(const IteratorImpl& other) const {
      return _node == other._node;
    }

    bool operator!=(const IteratorImpl& rhs) const {
      return !(*this == rhs);
    }

    ReturnType operator*() {
      assert(_node && "dereference end iterator");
      return { _node->_key, _node->_value };
    }

    friend class SkipList;
  };

  Node* _root;
  size_t _size;

  std::vector<Node*> GetPredecessors(const Key& key) const {
    const Predicate cmp{};
    std::vector<Node*> predecessors;
    Node* curr = _root;
    while (curr) {
      while (curr->_next && cmp(curr->_next->_key, key)) {
        curr = curr->_next;
      }
      predecessors.push_back(curr);
      curr = curr->_below;
    }
    return predecessors;
  }

public:
  using Iterator = IteratorImpl<std::pair<const Key&, Value&>>;
  using ConstIterator = IteratorImpl<std::pair<const Key&, const Value&>>;

  Iterator Search(const Key& key) {
    Node* node = GetPredecessors(key).back()->_next;
    if (node && node->_key == key) {
      while (node->_below) {
        node = node->_below;
      }
      return Iterator(static_cast<ValueNode*>(node));
    }
    return Iterator(nullptr);
  }

  ConstIterator Search(const Key& key) const {
    Node* node = GetPredecessors(key).back()->_next;
    if (node && node->_key == key) {
      while (node->_below) {
        node = node->_below;
      }
      return ConstIterator(static_cast<ValueNode*>(node));
    }
    return ConstIterator(nullptr);
  }

  void Insert(const Key& key, const Value& value) {

    ++_size;
  }

  SkipList() {

  }
};

