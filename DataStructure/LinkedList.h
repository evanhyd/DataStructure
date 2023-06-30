#pragma once
#include <utility>
#include <cassert>

template <typename T>
class LinkedList {
  class Node {
    T _data;
    Node* _next;

    template <typename ...Args>
    Node(Args&&... args) : _data(std::forward<Args>(args)...), _next(nullptr) {}

    friend class LinkedList;
  };

  template <typename ReturnType>
  class IteratorImpl {
    Node* node_;
    IteratorImpl(Node* node) : node_(node) {}

  public:
    ReturnType& operator*() {
      assert(node_ && "dereference null iterator");
      return node_->_data;
    }

    IteratorImpl& operator++() {
      assert(node_ && "dereference null iterator");
      node_ = node_->_next;
      return *this;
    }

    IteratorImpl operator++(int) {
      IteratorImpl old = *this;
      ++this;
      return old;
    }

    bool operator==(const IteratorImpl& rhs) const {
      return node_ == rhs.node_;
    }

    bool operator!=(const IteratorImpl& rhs) const {
      return !(node_ == rhs.node_);
    }

    friend class LinkedList;
  };

  Node* _front;
  Node* _back;
  size_t _size;

public:
  using Iterator = IteratorImpl<T&>;
  using ConstIterator = IteratorImpl<const T&>;

  LinkedList() : _size(0), _front(), _back() {};

  LinkedList(const LinkedList& rhs) {
    for (const auto& node : rhs) {
      PushBack(node);
    }
  }

  LinkedList(LinkedList&& rhs) noexcept : 
    _front(std::exchange(rhs._front, nullptr)),
    _back(std::exchange(rhs._back, nullptr)),
    _size(std::exchange(rhs._size, 0)) {
  }

  ~LinkedList() {
    while (_front) {
      Node* next = _front->_next;
      delete _front;
      _front = next;
    }
  }

  LinkedList& operator=(LinkedList rhs) {
    LinkedList temp(std::move(rhs));
    std::swap(*this, temp);
    return *this;
  }

  template <typename ...Args>
  void PushFront(Args&&... args) {
    Node* node = new Node(std::forward<Args>(args)...);
    node->_next = _front;
    _front = node;
    if (_size == 0) {
      assert(!_back);
      _back = _front;
    }
    ++_size;
  }

  template <typename... Args>
  void PushBack(Args&&... args) {
    Node* node = new Node(std::forward<Args>(args)...);
    if (_size == 0) {
      assert(!_front);
      _front = node;
    } else {
      _back->_next = node;
    }
    _back = node;
    ++_size;
  }

  void PopFront() {
    assert(_size > 0 && "pop front from an empty list");
    Node* old = _front;
    _front = _front->_next;
    if (--_size == 0) {
      _back = nullptr;
    }
    delete old;
  }
  
  Iterator begin() { return Iterator(_front); }
  Iterator end() { return Iterator(nullptr); }
  ConstIterator begin() const { return ConstIterator(_front); }
  ConstIterator end() const { return ConstIterator(nullptr); }
  size_t size() const { return _size; }
};
