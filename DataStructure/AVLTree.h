#pragma once
#include <iostream>
#include <string>
#include <utility>


/*

AVL Tree Implementation

*/

template <typename T>
class AVLTree;

template <typename T>
void swap(AVLTree<T>& lhs, AVLTree<T>& rhs);

template <typename T>
class AVLTree {

  class Node;

  int size_;
  Node* root_;

public:
  AVLTree();
  AVLTree(const AVLTree& tree);
  AVLTree(AVLTree&& tree) noexcept;
  ~AVLTree();

  AVLTree& operator=(AVLTree rhs) noexcept;

  bool Empty() const;
  int Size() const;
  T* Min();
  T* Max();
  const T* Min() const;
  const T* Max() const;
  void Print() const;

  T* Find(const T& key);
  const T* Find(const T& key) const;

  template <typename Arg>
  void Insert(Arg&& key);
  void Erase(const T& key);

  friend void swap<T>(AVLTree& lhs, AVLTree& rhs);
};


template <typename T>
void swap(AVLTree<T>& lhs, AVLTree<T>& rhs) {
  using std::swap;
  swap(lhs.size_, rhs.size_);
  swap(lhs.root_, rhs.root_);
}

template <typename T>
AVLTree<T>::AVLTree() : size_(0), root_(nullptr) {}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& tree) : size_(tree.size_) {
  if (tree.root_) {
    root_ = new Node(*tree.root_);
  }
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& tree) noexcept : size_(tree.size_), root_(std::exchange(tree.root_, nullptr)) {}

template <typename T>
AVLTree<T>::~AVLTree() {
  delete this->root_;
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree<T> tree) noexcept {
  swap(*this, tree);
  return *this;
}

template <typename T>
bool AVLTree<T>::Empty() const {
  return size_ == 0;
}

template <typename T>
int AVLTree<T>::Size() const {
  return size_;
}

template <typename T>
const T* AVLTree<T>::Min() const {
  return (root_ ? root_->Min() : nullptr);
}

template <typename T>
T* AVLTree<T>::Min() {
  return const_cast<T*>(static_cast<const AVLTree<T>&>(*this).Min());
}

template <typename T>
const T* AVLTree<T>::Max() const {
  return (root_ ? root_->Max() : nullptr);
}

template <typename T>
T* AVLTree<T>::Max() {
  return const_cast<T*>(static_cast<const AVLTree<T>&>(*this).Max());
}

template <typename T>
void AVLTree<T>::Print() const {

  if (!this->root_) {
    std::cout << "Empty Tree\n";
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

template <typename T>
const T* AVLTree<T>::Find(const T& key) const {
  return Node::Find(root_, key);
}

template <typename T>
T* AVLTree<T>::Find(const T& key) {
  return const_cast<T*>(static_cast<const AVLTree<T>&>(*this).Find(key));
}

template <typename T>
template <typename Arg>
void AVLTree<T>::Insert(Arg&& key) {
  Node::Insert(&root_, root_, std::forward<Arg>(key));
}

template <typename T>
void AVLTree<T>::Erase(const T& key) {
  Node::Erase(&root_, root_, key);
}








/*

AVL Tree Node implementation

*/


template <typename T>
class AVLTree<T>::Node {

  short height_;
  short balance_factor_;
  T key_;
  Node* left_;
  Node* right_;

public:
  Node(const T& key);
  Node(T&& key);
  Node(const Node& rhs);
  Node(Node&& rhs) noexcept;
  ~Node();

  Node& operator=(const Node& rhs) = delete;
  Node& operator=(Node&& rhs) = delete;

  T* Min();
  T* Max();
  const T* Min() const;
  const T* Max() const;
  std::string Print(const int depth, int& max_depth, int& max_breadth);


  static T* Find(Node* curr, const T& key);
  static const T* Find(const Node* curr, const T& key);

  template <typename Arg>
  static void Insert(Node** parent, Node* curr, Arg&& key);
  static void Erase(Node** parent, Node* curr, const T& key);
  static void EraseReplacement(Node** parent, Node* curr, Node* root);
  static void BalanceSubtree(Node** parent, Node* root);

private:
  void UpdateInfo();
  void RotateLeftLeft(Node** parent);
  void RotateLeftRight(Node** parent);
  void RotateRightRight(Node** parent);
  void RotateRightLeft(Node** parent);
};



template <typename T>
AVLTree<T>::Node::Node(const T& key) : 
  height_(0), balance_factor_(0), key_(key), left_(nullptr), right_(nullptr) {}

template <typename T>
AVLTree<T>::Node::Node(T&& key) : 
  height_(0), balance_factor_(0), key_(std::move(key)), left_(nullptr), right_(nullptr) {}

template <typename T>
AVLTree<T>::Node::Node(const Node& rhs) : 
  height_(rhs.height_), balance_factor_(rhs.balance_factor_), key_(rhs.key_) {
  left_ = (rhs.left_ ? new Node(*rhs.left_) : nullptr);
  right_ = (rhs.right_ ? new Node(*rhs.right_) : nullptr);
}

template <typename T>
AVLTree<T>::Node::Node(Node&& rhs) noexcept :
  height_(rhs.height_), balance_factor_(rhs.balance_factor_), key_(std::move(rhs.key_)), left_(std::exchange(rhs.left_, nullptr), right_(std::exchange(rhs.right_, nullptr))) {}

template <typename T>
AVLTree<T>::Node::~Node() {
  delete this->left_;
  delete this->right_;
}

template <typename T>
const T* AVLTree<T>::Node::Min() const {
  return (this->left_ ? this->left_->Min() : &this->key_);
}

template <typename T>
T* AVLTree<T>::Node::Min() {
  return const_cast<T*>(static_cast<const AVLTree<T>::Node&>(*this).Min());
}

template <typename T>
const T* AVLTree<T>::Node::Max() const {
  return (this->right_ ? this->right_->Max() : &this->key_);
}

template <typename T>
T* AVLTree<T>::Node::Max() {
  return const_cast<T*>(static_cast<const AVLTree<T>::Node&>(*this).Max());
}

template <typename T>
std::string AVLTree<T>::Node::Print(const int depth, int& max_depth, int& max_breadth) {

  //std::string tree_str(std::to_string(this->height_) + "(" + std::to_string(this->balance_factor_) + ")");
  std::string tree_str(std::to_string(this->key_));
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
const T* AVLTree<T>::Node::Find(const Node* curr, const T& key) {
  if (!curr) {
    return nullptr;
  }

  if (key < curr->key_) {
    return Find(curr->left_, key);
  } else if (curr->key_ < key) {
    return Find(curr->right_, key);
  } else {
    return &curr->key_;
  }
}

template <typename T>
T* AVLTree<T>::Node::Find(Node* curr, const T& key) {
  return const_cast<T*>(Find(static_cast<const Node*>(curr), key));
}

template <typename T>
template <typename Arg>
void AVLTree<T>::Node::Insert(Node** parent, Node* curr, Arg&& key) {

  //null leaf
  if (!curr) {
    (*parent) = new Node(std::forward<Arg>(key));
    return;
  }

  //check subtree
  if (key < curr->key_) {
    Insert(&curr->left_, curr->left_, std::forward<Arg>(key));
  } else if (curr->key_ < key) {
    Insert(&curr->right_, curr->right_, std::forward<Arg>(key));
  } else {
    return;
  }

  //balance the subtree
  BalanceSubtree(parent, curr);
}

template <typename T>
void AVLTree<T>::Node::Erase(Node** parent, Node* curr, const T& key) {

  //key does not exist
  if (!curr) {
    return;
  }

  if (key < curr->key_) {

    //check left subtree
    Erase(&curr->left_, curr->left_, key);

  } else if (curr->key_ < key) {

    //check right subtree
    Erase(&curr->right_, curr->right_, key);

  } else {

    //found the node
    if (curr->left_) {

      //max node in left subtree
      EraseReplacement(&curr->left_, curr->left_, curr);
      BalanceSubtree(parent, curr);

    } else if (curr->right_) {

      //right child only
      *parent = std::exchange(curr->right_, nullptr);
      delete curr;

    } else {

      //leaf node
      *parent = nullptr;
      delete curr;
    }

    return;
  }

  BalanceSubtree(parent, curr);
}

template <typename T>
void AVLTree<T>::Node::EraseReplacement(Node** parent, Node* curr, Node* root) {

  //finding the max node in the subtree
  if (curr->right_) {
    EraseReplacement(&curr->right_, curr->right_, root);
    BalanceSubtree(parent, root);
  } else {
    root->key_ = std::move(curr->key_);
    *parent = std::exchange(curr->left_, nullptr);
    delete curr;
  }
}

template <typename T>
void AVLTree<T>::Node::BalanceSubtree(Node** parent, Node* root) {

  //update height and balance factor
  root->UpdateInfo();

  //rotate based on balanced factor
  if (root->balance_factor_ <= -2) {

    //left heavy
    assert(root->left_);

    if (root->left_->balance_factor_ <= 0) {
      root->RotateLeftLeft(parent);
    } else {
      root->RotateLeftRight(parent);
    }

  } else if (root->balance_factor_ >= 2) {

    //right heavy
    assert(root->right_);

    if (root->right_->balance_factor_ >= 0) {
      root->RotateRightRight(parent);
    } else {
      root->RotateRightLeft(parent);
    }
  }

  //check post balance factor
  assert((root->right_ ? root->right_->height_ : -1) - (root->left_ ? root->left_->height_ : -1) == root->balance_factor_);
}



template <typename T>
void AVLTree<T>::Node::UpdateInfo() {
  const short left_height = (this->left_ ? this->left_->height_ : -1);
  const short right_height = (this->right_ ? this->right_->height_ : -1);
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
  RL->right_ = R;
  RL->left_ = this;

  this->UpdateInfo();
  (*parent)->right_->UpdateInfo();
  (*parent)->UpdateInfo();
}