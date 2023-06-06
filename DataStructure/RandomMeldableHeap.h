#pragma once
#include <cassert>
#include <utility>
#include <random>
#include <iostream>

template <typename T, typename Predicate = std::greater<T>>
class RandomMeldableHeap {
  struct Node {
    T val_;
    Node* left_;
    Node* right_;

    Node(T&& val) : val_(std::move(val)), left_(), right_() {}
    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
    ~Node() {
      delete left_;
      delete right_;
    }
  };

  Node* root_;
  size_t size_;

public:
  RandomMeldableHeap() : root_(), size_() {}
  RandomMeldableHeap(const RandomMeldableHeap&) = delete;
  RandomMeldableHeap(RandomMeldableHeap&& heap) : root_(std::exchange(heap.root_, nullptr)), size_(std::exchange(heap.size_, 0)) {}
  RandomMeldableHeap& operator=(const RandomMeldableHeap&) = delete;
  RandomMeldableHeap& operator=(RandomMeldableHeap&&) = delete;
  ~RandomMeldableHeap() {
    delete root_;
  }

private:
  static Node* MergeRoots(Node* root0, Node* root1, Predicate p = Predicate()) {
    if (!root0) {
      return root1;
    }
    if (!root1) {
      return root0;
    }

    if (p(root1->val_, root0->val_)) {
      std::swap(root0, root1);
    }

    Node** child = (dis(rd) ? &root0->left_ : &root0->right_);
    *child = MergeRoots(root1, *child);
    return root0;
  }

public:
  void Merge(RandomMeldableHeap&& heap) {
    root_ = MergeRoots(root_, heap.root_);
    size_ += heap.size_;
    heap.root_ = nullptr;
    heap.size_ = 0;
  }

  template <typename ...Args>
  void Push(Args&&... args) {
    root_ = MergeRoots(root_, new Node(T(std::forward<Args>(args)...)));
    ++size_;
  }

  T& Top() {
    assert(size_ >= 0 && "access empty heap top");
    return root_->val_;
  }

  const T& Top() const {
    assert(size_ >= 0 && "access empty heap top");
    return root_->val_;
  }

  void Pop() {
    assert(root_ && "pop from empty heap");

    Node* oldRoot = root_;
    root_ = MergeRoots(root_->left_, root_->right_);
    oldRoot->left_ = nullptr;
    oldRoot->right_ = nullptr;
    delete oldRoot;
    --size_;
  }

  size_t Size() const {
    return size_;
  }

private:
  inline static std::random_device rd{};
  inline static std::bernoulli_distribution dis{};
};

