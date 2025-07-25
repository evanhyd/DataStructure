#pragma once
#include <iostream>
#include <map>
#include <vector>

template <typename T>
class Trie {

//public:
  class Node {
  public:
    std::map<T, Node> child_;
    bool is_end_;
    Node();
  };

private:
  Node root_;

public:
  Trie();
  Node* Find(const T* arr, std::size_t len);
  const Node* Find(const T* arr, std::size_t len) const;
  bool Contain(const T* arr, std::size_t len) const;
  bool ContainPrefix(const T* arr, std::size_t len) const;
  void Push(const T* arr, std::size_t len);
  void Erase(const T* arr, std::size_t len);
  std::vector<std::vector<T>> GetAll() const;

private:
  static void Walk(std::vector< std::vector<T>>& res, std::vector<T>& seq, const Trie<T>::Node& curr);
};




template <typename T>
Trie<T>::Trie() : root_() {}

template<typename T>
Trie<T>::Node::Node() : child_(), is_end_(false) {}


template <typename T>
typename Trie<T>::Node* Trie<T>::Find(const T* arr, std::size_t len) {
  return const_cast<Trie<T>::Node*>(static_cast<const Trie<T>&>(*this).Find(arr, len));
}

template <typename T>
const typename Trie<T>::Node* Trie<T>::Find(const T* arr, std::size_t len) const {
  const typename Trie<T>::Node* curr = &root_;
  for (std::size_t i = 0; i < len; ++i) {
    const auto res = curr->child_.find(arr[i]);
    if (res == curr->child_.end()) return nullptr;
    curr = &res->second;
  }
  return curr;
}


template <typename T>
bool Trie<T>::Contain(const T* arr, std::size_t len) const {
  const typename Trie<T>::Node* res = Find(arr, len);
  return res != nullptr && res->is_end_;
}

template <typename T>
bool Trie<T>::ContainPrefix(const T* arr, std::size_t len) const {
  const typename Trie<T>::Node* res = Find(arr, len);
  return res != nullptr;
}

template <typename T>
void Trie<T>::Push(const T* arr, std::size_t len)  {
  Trie<T>::Node* curr = &root_;
  for (std::size_t i = 0; i < len; ++i) {
    curr = &curr->child_[arr[i]];
  }
  curr->is_end_ = true;
}

template <typename T>
void Trie<T>::Erase(const T* arr, std::size_t len) {

  std::vector<Trie<T>::Node*> stk;
  Trie<T>::Node* curr = &root_;
  for (std::size_t i = 0; i < len; ++i) {
    auto res = curr->child_.find(arr[i]);
    if (res == curr->child_.end()) return;
    stk.push_back(curr);
    curr = &res->second;
  }

  //check if such string is in the trie
  if (!curr->is_end_) return;
  curr->is_end_ = false;

  
  for (int i = int(len) - 1; i >= 0; --i) {
    
    //erase from the parent if node is not ended and no child
    if (curr->child_.empty() && !curr->is_end_) {
      stk[i]->child_.erase(arr[i]);
      curr = stk[i];
    } else {
      break;
    }
  }
}


template <typename T>
std::vector<std::vector<T>> Trie<T>::GetAll() const {

  std::vector<std::vector<T>> res;
  std::vector<T> seq;
  Walk(res, seq, root_);
  return res;
}


template <typename T>
void Trie<T>::Walk(std::vector< std::vector<T>>& res, std::vector<T>& seq, const Trie<T>::Node& curr) {
  for (const auto& child : curr.child_) {

    //extend the trie sequence
    seq.push_back(child.first);
    if (child.second.is_end_) {
      res.push_back(seq);
    }

    //keep walking down the tree
    Walk(res, seq, child.second);

    //go back to the parent
    seq.pop_back();
  }
}