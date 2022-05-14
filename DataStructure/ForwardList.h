#pragma once
#include <algorithm>

template <typename T>
class ForwardList;

template <typename T>
void swap(ForwardList<T>& lhs, ForwardList<T>& rhs);



template <typename T>
class ForwardList
{
    class Node
    {
    public:
        T val_;
        Node* next_;
    };


    Node *head_;
    int size_; 

public:

    ForwardList();
    ForwardList(const ForwardList& lst);
    ForwardList(ForwardList&& lst) noexcept;
    ~ForwardList();

    ForwardList& operator=(ForwardList lst);


   
    int Size() const;
    bool IsEmpty() const;
    
    void PushFront(const T& item);
    void PopFront();

    void InsertAfter(Node* node, const T& item);
    void DeleteAfter(Node* node);
    void Clear();


    T& Front();
    const T& Front() const;
    Node* Head();
    const Node* Head() const;

    void Reverse();
    //void Sort();


    friend void swap<T>(ForwardList<T>& lhs, ForwardList<T>& rhs);
};


template <typename T>
void swap(ForwardList<T>& lhs, ForwardList<T>& rhs)
{
    using std::swap;
    swap(lhs.head_, rhs.head_);
    swap(lhs.size_, rhs.size_);
}


//constructor
template <typename T>
ForwardList<T>::ForwardList() : head_(nullptr), size_(0)
{
    //empty
}

template <typename T>
ForwardList<T>::ForwardList(const ForwardList<T>& lst) : ForwardList()
{
    Node** back_inserter = &head_;

    Node* node = lst.head_;
    while (node)
    {
        (*back_inserter) = new Node(node->val_, nullptr);
        back_inserter = (*back_inserter)->next_;
        node = node->next_;
    }
}

template <typename T>
ForwardList<T>::ForwardList(ForwardList<T>&& lst) noexcept : ForwardList()
{
    swap(*this, lst);
}



//destructor
template <typename T>
ForwardList<T>::~ForwardList()
{
    this->Clear();
}



//assignment operators
template <typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList<T> lst)
{
    swap(*this, lst);
}






template <typename T>
int ForwardList<T>::Size() const
{
    return size_;
}

template <typename T>
bool ForwardList<T>::IsEmpty() const
{
    return size_ == 0;
}



template <typename T>
void ForwardList<T>::PushFront(const T& item)
{
    head_ = new Node{ item, head_ };
    ++size_;
}

template <typename T>
void ForwardList<T>::PopFront()
{
    assert(head_);
    auto old_head = head_;
    head_ = head_->next_;
    delete old_head;

    --size_;
}




template <typename T>
void ForwardList<T>::InsertAfter(Node* node, const T& item)
{
    assert(node);
    Node* next_child = node->next_;
    node->next_ = new Node{ item, next_child };
}

template <typename T>
void ForwardList<T>::DeleteAfter(Node* node)
{
    assert(node);
    Node* to_delete = node->next_;
    if (to_delete == nullptr) return;
    
    node->next_ = to_delete->next_;
    delete to_delete;
}

template <typename T>
void ForwardList<T>::Clear()
{
    while (head_)
    {
        this->PopFront();
    }
}




template <typename T>
T& ForwardList<T>::Front()
{
    assert(!this->IsEmpty());
    return head_->val_;
}

template <typename T>
const T& ForwardList<T>::Front() const
{
    assert(!this->IsEmpty());
    return head_->val_;
}

template <typename T>
auto ForwardList<T>::Head() -> Node*
{
    return head_;
}

template <typename T>
auto ForwardList<T>::Head() const -> const Node*
{
    return head_;
}




template <typename T>
void ForwardList<T>::Reverse()
{
    Node* prev = nullptr;
    Node* child = nullptr;

    while (head_)
    {
        child = head_->next_;
        head_->next_ = prev;
        prev = head_;
        head_ = child;
    }

    head_ = prev;
}


//template <typename T>
//void ForwardList<T>::Sort()
//{
//
//}
