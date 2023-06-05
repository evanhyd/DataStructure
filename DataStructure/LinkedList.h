#pragma once
#include <utility>
#include <cassert>

#include "memory.h"

template <typename T>
class LinkedList {
  struct Node {
    T data_;
    Node* next_;
    Node(T&& data, Node* next) : data_(std::move(data)), next_(next) {}
  };

  template <typename ValueType, typename NodeType>
  struct IteratorImpl {
    NodeType* node_;

    IteratorImpl(NodeType* node) : node_(node) {}

    ValueType& operator*() {
      assert(node_ && "dereference null iterator");
      return node_->data_;
    }

    IteratorImpl& operator++() {
      assert(node_ && "dereference null iterator");
      node_ = node_->next_;
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
  };

public:
  using Iterator = IteratorImpl<T, Node>;
  using ConstIterator = IteratorImpl<const T, const Node>;

private:
  Node* front_;
  Node* back_;
  size_t size_;

public:
  LinkedList() : size_(0), front_(), back_(){
  };

  LinkedList(const LinkedList& rhs) {
    for (const auto& node : rhs) {
      PushBack(node);
    }
  }

  LinkedList(LinkedList&& rhs) noexcept : 
    front_(std::exchange(rhs.front_, nullptr)),
    back_(std::exchange(rhs.back_, nullptr)),
    size_(std::exchange(rhs.size_, 0)) {
  }

  ~LinkedList() {
    while (front_) {
      Node* next = front_->next_;
      delete front_;
      front_ = next;
    }
  }

  LinkedList& operator=(LinkedList rhs) {
    LinkedList temp(std::move(rhs));
    std::swap(*this, temp);
    return *this;
  }

  template <typename ...Args>
  void PushFront(Args&&... args) {
    front_ = new Node(T(std::forward<Args>(args)...), front_);
    //front_ = box::Pool::Allocate<Node>(1, T(std::forward<Args>(args)...), front_);
    if (size_ == 0) {
      assert(!back_);
      back_ = front_;
    }
    ++size_;
  }

  template <typename... Args>
  void PushBack(Args&&... args) {
    Node* node = new Node(T(std::forward<Args>(args)...), nullptr);
    //Node* node = box::Pool::Allocate<Node>(1, T(std::forward<Args>(args)...), nullptr);
    if (size_ == 0) {
      assert(!front_);
      front_ = node;
    } else {
      back_->next_ = node;
    }
    back_ = node;
    ++size_;
  }

  void PopFront() {
    assert(size_ > 0 && "pop front from an empty list");
    Node* old = front_;
    front_ = front_->next_;
    if (--size_ == 0) {
      back_ = nullptr;
    }
    delete old;
  }
  
  Iterator begin() { return Iterator(front_); }
  Iterator end() { return Iterator(nullptr); }
  ConstIterator begin() const { return ConstIterator(front_); }
  ConstIterator end() const { return ConstIterator(nullptr); }
  size_t size() const { return size_; }
};
