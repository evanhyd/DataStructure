#pragma once
#include <utility>
#include <string>
#include <iostream>

template <typename T>
class AVLTree {
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
    void Insert(Arg&& key, bool replace, AVLTree<T>::Node** parent);
    std::string Print(const int depth, int& max_depth, int& max_breadth);

  private:
    void UpdateInfo();
    void RotateLeftLeft(Node** parent);
    void RotateLeftRight(Node** parent);
    void RotateRightRight(Node** parent);
    void RotateRightLeft(Node** parent);
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

  void Print() const;
};

template <typename T>
AVLTree<T>::Node::Node(const T& key) : height_(0), balance_factor_(0), key_(key), left_(nullptr), right_(nullptr) {}

template <typename T>
AVLTree<T>::Node::Node(T&& key) : height_(0), balance_factor_(0), key_(std::move(key)), left_(nullptr), right_(nullptr) {}

template <typename T>
T* AVLTree<T>::Node::Find(const T& key) {
  return const_cast<T*>(static_cast<const AVLTree<T>::Node&>(*this).Find(key));
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
void AVLTree<T>::Node::Insert(Arg&& key, bool replace, AVLTree<T>::Node** parent) {

  //found
  if (key == this->key_) {
    if (replace) {
      this->key_ = std::forward<Arg>(key);
    }
    return;

  } else if (key < this->key_) {
    if (this->left_) {
      this->left_->Insert(std::forward<Arg>(key), replace, &this->left_);
    } else {
      this->left_ = new Node(std::forward<Arg>(key));
    }

  } else {
    if (this->right_) {
      this->right_->Insert(std::forward<Arg>(key), replace, &this->right_);
    } else {
      this->right_ = new Node(std::forward<Arg>(key));
    }
  }

  //update height and balance factor
  this->UpdateInfo();

  //rotate based on balanced factor
  if (this->balance_factor_ <= -2) {

    //left heavy
    assert(this->left_);
    assert(this->left_->balance_factor_ != 0);

    if (this->left_->balance_factor_ < 0) {
      this->RotateLeftLeft(parent);
    } else {
      this->RotateLeftRight(parent);
    }
  } else if (this->balance_factor_ >= 2) {

    //right heavy
    assert(this->right_);
    assert(this->right_->balance_factor_ != 0);

    if (this->right_->balance_factor_ > 0) {
      this->RotateRightRight(parent);
    } else {
      this->RotateRightLeft(parent);
    }
  }

  return;
}

template <typename T>
void AVLTree<T>::Node::UpdateInfo() {
  const short left_height = (this->left_ ? this->left_->height_: -1);
  const short right_height = (this->right_ ? this->right_->height_: -1);
  this->height_ = std::max(left_height, right_height) + 1;
  this->balance_factor_ = right_height - left_height;
}

template <typename T>
void AVLTree<T>::Node::RotateLeftLeft(Node** parent) {

  Node* const L = this->left_;

  *parent = L;
  this->left_ = L->right_;
  L->right_ = this;

  //update height and balance factor for this node and new parent node
  //left subtree remain the same
  this->UpdateInfo();
  (*parent)->UpdateInfo();
}

template <typename T>
void AVLTree<T>::Node::RotateLeftRight(Node** parent) {

  Node* const L = this->left_;
  Node* const LR = L->right_;

  *parent = LR;
  L->right_ = LR->left_;
  this->left_ = LR->right_;
  LR->left_ = L;
  LR->right_ = this;

  this->UpdateInfo();
  (*parent)->left_->UpdateInfo();
  (*parent)->UpdateInfo();
}

template <typename T>
void AVLTree<T>::Node::RotateRightRight(Node** parent) {

  Node* const R = this->right_;

  *parent = R;
  this->right_ = R->left_;
  R->left_ = this;

  this->UpdateInfo();
  (*parent)->UpdateInfo();
}

template <typename T>
void AVLTree<T>::Node::RotateRightLeft(Node** parent) {
  Node* const R = this->right_;
  Node* const RL = R->left_;

  *parent = RL;
  R->left_ = RL->right_;
  this->right_ = RL->left_;
  RL->right_= R;
  RL->left_ = this;

  this->UpdateInfo();
  (*parent)->right_->UpdateInfo();
  (*parent)->UpdateInfo();
}


template <typename T>
std::string AVLTree<T>::Node::Print(const int depth, int& max_depth, int& max_breadth) {

  std::string tree_str(std::to_string(this->height_));
  if (this->left_ || this->right_) {
    tree_str += " -> ";
  }

  const std::string space = std::string((depth + 1) * 5, ' ') + "|";

  if (this->right_) {
    tree_str += "\n" + space + this->right_->Print(depth + 1, max_depth, max_breadth);
  }
  if (this->left_) {
    tree_str += "\n" + space + this->left_->Print(depth + 1, max_depth, max_breadth);
  }

  //update the max_depth
  max_depth = std::max(max_depth, depth);
  max_breadth = std::max(max_breadth, int(this->height_));

  return tree_str;
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
  return const_cast<T*>(static_cast<const AVLTree<T>&>(*this).Find(key));
}

template <typename T>
const T* AVLTree<T>::Find(const T& key) const {
  return (root_ ? root_->Find(key) : nullptr);
}

template <typename T>
template <typename Arg>
void AVLTree<T>::Insert(Arg&& key, bool replace) {
  if (root_) {
    root_->Insert(std::forward<Arg>(key), replace, &this->root_);
  } else {
    root_ = new Node(std::forward<Arg>(key));
  }
}

template <typename T>
bool AVLTree<T>::Erase(const T& key) {}

template <typename T>
void AVLTree<T>::Print() const {

  if (!this->root_) {
    return;
  }

  int max_depth = 0;
  int max_breadth = 0;
  std::string graph_str = this->root_->Print(0, max_depth, max_breadth);
  std::string line_break(max_depth * 6, '-');

  std::cout << '\n'
    << graph_str << '\n'
    << line_break << '\n'
    << "Max Depth: " << max_depth << '\n'
    << "Max Breadth: " << max_breadth << '\n';
}
