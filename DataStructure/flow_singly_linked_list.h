#pragma once
#include "flow_polymorphic_allocator.h"
#include <memory>
#include <utility>
#include <cassert>

namespace flow {
  template <typename T, typename Allocator = PolymorphicAllocator<>>
  class SinglyLinkedList {
  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<T>;;

  private:
    using allocator_trait = std::allocator_traits<Allocator>::template rebind_traits<T>;

    struct Node {
      Node* next;
      T value;
    };

    template <typename ValueType>
    class IteratorImp {
      Node* node_;
      IteratorImp(Node* node)
        : node_(node) {
      }

    public:
      using value_type = T;

      ValueType& operator*() {
        assert(node_ && "dereference null iterator");
        return node_->value;
      }

      IteratorImp& operator++() {
        assert(node_ && "dereference null iterator");
        node_ = node_->next;
        return *this;
      }

      IteratorImp operator++(int) {
        assert(node_ && "dereference null iterator");
        IteratorImp original = *this;
        ++(*this);
        return original;
      }

      bool operator==(const IteratorImp& rhs) const {
        return node_ == rhs.node_;
      }

      bool operator!=(const IteratorImp& rhs) const {
        return !(*this == rhs);
      }

      friend SinglyLinkedList;
    };

    allocator_type allocator_;
    Node* head_;
    Node* tail_;
    std::size_t size_;

  public:
    IteratorImp<T> begin() {
      return IteratorImp<T>(head_);
    }

    IteratorImp<const T> begin() const {
      return IteratorImp<const T>(head_);
    }
  };
}