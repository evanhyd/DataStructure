#pragma once
#include <utility>

template <typename T>
class AVLTree
{
  class Node {
    short height_;
    short balance_factor_;
    T key_;
    Node* left_;
    Node* right_;

  public:
    Node(const T& key);
    Node(T&& key);

    T* Find(const T& key);
    const T* Find(const T& key) const;
    void Insert(const T& key, bool replace = false);
  };

  int size_;
  Node* root_;

public:
  AVLTree();

  bool Empty() const;
  int Size() const;

  T* Find(const T& key);
  const T* Find(const T& key) const;
  void Insert(const T& key, bool replace = false);
  void Insert(T&& key, bool replace = false);
  bool Erase(const T& key);

  void PrintTree() const;
};

template <typename T>
AVLTree<T>::Node::Node(const T& key) : height_(0), balance_factor_(0), key_(key), left_(nullptr), right_(nullptr) {}

template <typename T>
AVLTree<T>::Node::Node(T&& key) : height_(0), balance_factor_(0), key_(std::move(key)), left_(nullptr), right_(nullptr) {}

template <typename T>
T* AVLTree<T>::Node::Find(const T& key) {
  if (key == this->key_) {
    return &this->key_;
  } else if (key < this->key_) {
    return (this->left_ ? this->left_->Find(key) : nullptr);
  } else {
    return (this->right_ ? this->right_->Find(key) : nullptr);
  }
}

template <typename T>
const T* AVLTree<T>::Node::Find(const T& key) const {
  if (key == this->key_) {
    return &this->key_;
  } else if (key < this->key_) {
    return (this->left_ ? this->left_->Find(key) : nullptr);
  } else {
    return (this->right_ ? this->right_->Find(key) : nullptr);
  }
}

template <typename T>
void AVLTree<T>::Node::Insert(const T& key, bool replace) {

  //found
  if (key == this->key_) {
    if (replace) {
      this->key_ = key;
    }
    return;

  } else if (key < this->key_) {
    if (this->left_) {
      this->left_->Insert(key, replace);
    } else {
      this->left_ = new Node(key);
    }

  } else {
    if (this->right_) {
      this->right_->Insert(key, replace);
    } else {
      this->right_ = new Node(key);
    }
  }

  //update height
  const int left_height = (this->left_ ? this->left_->height_ : 0);
  const int right_height = (this->right_ ? this->right_->height_ : 0);
  this->height_ = std::max(left_height, right_height) + 1;
  this->balance_factor_ = right_height - left_height;

  //rotate based on balanced factor
  if (this->balance_factor_ <= -2) {

    //left heavy
    assert(this->left_);
    assert(this->left_->balance_factor_ != 0);

    if (this->left_->balance_factor_ < 0) {
      //LL
      //right rotation
    } else {
      //LR
      //left rotation + right rotation
    }
  } else if (this->balance_factor_ >= 2) {

    //right heavy
    assert(this->right_);
    assert(this->right_->balance_factor_ != 0);

    if (this->right_->balance_factor_ > 0) {
      //RR
      //left rotation
    } else {
      //RL
      //right rotation + left rotation
    }
  }

  return;
}





template <typename T>
AVLTree<T>::AVLTree() : size_(0), root_(nullptr) {}

template <typename T>
bool AVLTree<T>::Empty() const {
  return size_ == 0;
}

template <typename T>
int AVLTree<T>::Size() const {
  return size_;
}

template <typename T>
T* AVLTree<T>::Find(const T& key) {
  return (root_ ? root_->Find(key) : nullptr);
}

template <typename T>
const T* AVLTree<T>::Find(const T& key) const {
  return (root_ ? root_->Find(key) : nullptr);
}

template <typename T>
void AVLTree<T>::Insert(const T& key, bool replace) {
  if (root_) {
    root_->Insert(key, replace);
  } else {
    root_ = new Node(key);
  }
}

template <typename T>
void AVLTree<T>::Insert(T&& key, bool replace) {}

template <typename T>
bool AVLTree<T>::Erase(const T& key) {}

template <typename T>
void AVLTree<T>::PrintTree() const {}
