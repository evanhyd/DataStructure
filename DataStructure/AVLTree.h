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

    template <typename Arg>
    void Insert(Arg&& key, bool replace = false);
  };

  int size_;
  Node* root_;

public:
  AVLTree();

  bool Empty() const;
  int Size() const;

  T* Find(const T& key);
  const T* Find(const T& key) const;

  template <typename Arg>
  void Insert(Arg&& key, bool replace = false);
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
template <typename Arg>
void AVLTree<T>::Node::Insert(Arg&& key, bool replace) {

  //found
  if (key == this->key_) {
    if (replace) {
      this->key_ = std::forward<Arg>(key);
    }
    return;

  } else if (key < this->key_) {
    if (this->left_) {
      this->left_->Insert(std::forward<Arg>(key), replace);
    } else {
      this->left_ = new Node(std::forward<Arg>(key));
    }

  } else {
    if (this->right_) {
      this->right_->Insert(std::forward<Arg>(key), replace);
    } else {
      this->right_ = new Node(std::forward<Arg>(key));
    }
  }

  //update height
  const short left_height = (this->left_ ? this->left_->height_ : 0);
  const short right_height = (this->right_ ? this->right_->height_ : 0);
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
template <typename Arg>
void AVLTree<T>::Insert(Arg&& key, bool replace) {
  if (root_) {
    root_->Insert(std::forward<Arg>(key), replace);
  } else {
    root_ = new Node(std::forward<Arg>(key));
  }
}

template <typename T>
bool AVLTree<T>::Erase(const T& key) {}

template <typename T>
void AVLTree<T>::PrintTree() const {}
