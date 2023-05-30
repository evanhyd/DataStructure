#pragma once
#include <utility>

template <typename T>
class LinkedList {
  class Node {
    T data_;
    Node* next_;
    Node(T&& data, Node* next) : data_(std::move(data)), next_(next) {}
    friend class LinkedList;
  };

  template <typename NodeType, typename ValueType>
  class IteratorImpl {
    NodeType* node_;
    IteratorImpl(NodeType* node) : node_(node) {}

   public:
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
      Iterator old = *this;
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

public:
  using Iterator = IteratorImpl<Node, T>;
  using ConstIterator = IteratorImpl<const Node, const T>;

private:
  size_t size_;
  Node* front_;
  Node* back_;

public:
  LinkedList() : size_(0), front_(), back_(){};

  template <typename ...Args>
  void PushFront(Args&&... args) {
    front_ = new Node(T(std::forward<Args>(args)...), front_);
    if (size_ == 0) {
      assert(!back_);
      back_ = front_;
    }
    ++size_;
  }

  template <typename... Args>
  void PushBack(Args&&... args) {
    if (size_ == 0) {
      assert(!front_);
      back_ = new Node(T(std::forward<Args>(args)...), nullptr);
      front_ = back_;
    } else {
      back_->next_ = new Node(T(std::forward<Args>(args)...), nullptr);
      back_ = back_->next_;
    }
    ++size_;
  }
  
  Iterator begin() { return Iterator(front_); }
  Iterator end() { return Iterator(nullptr); }
  ConstIterator begin() const { return ConstIterator(front_); }
  ConstIterator end() const { return ConstIterator(nullptr); }
  size_t size() const { return size_; }
};