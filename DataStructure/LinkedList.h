#pragma once

template <typename T>
class LinkedList;

template <typename T>
void swap(LinkedList<T>& lst0, LinkedList<T>& lst1);



template <typename T>
class LinkedList
{
    class Node
    {
    public:
        T val_;
        Node* next_;
    };


    Node *head_;
    Node *tail_;
    int size_; 

public:

    LinkedList();
    LinkedList(const LinkedList& lst);
    LinkedList(LinkedList&& lst) noexcept;
    ~LinkedList();

    LinkedList& operator=(LinkedList lst);


   
    int Size() const;
    bool IsEmpty() const;
    
    void PushFront(const T& item);
    void PushBack(const T& item);
    void PopFront();

    void Insert(Node* dest, const T& item);
    void Delete(Node* node);
    void Clear();


    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;
    Node* Head();
    const Node* Head() const;

    void Reverse();
    void Sort();


    friend void swap<T>(LinkedList<T>& lst0, LinkedList<T>& lst1);
};


template <typename T>
void swap(LinkedList<T>& lst0, LinkedList<T>& lst1)
{
    using namespace std;
    swap(lst0.head_, lst1.head_);
    swap(lst0.tail_, lst1.tail_);
    swap(lst0.size_, lst1.size_);
}


//constructor
template <typename T>
LinkedList<T>::LinkedList() : head_(nullptr), tail_(&head_), size_(0)
{
    //empty
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& lst) : LinkedList()
{
    auto node = lst.head_;
    while (node)
    {
        this->PushBack(node->val_);
        node = node->next_;
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& lst) noexcept : LinkedList()
{
    swap(*this, lst);
}




//destructor
template <typename T>
LinkedList<T>::~LinkedList()
{
    this->Clear();
}





//assignment operators
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T> lst)
{
    swap(*this, lst);
}












template <typename T>
int LinkedList<T>::Size() const
{
    return size_;
}

template <typename T>
bool LinkedList<T>::IsEmpty() const
{
    return size_ == 0;
}





template <typename T>
void LinkedList<T>::PushFront(const T& item)
{
    head_ = new Node{ item, head_ };
    if (!tail_) tail_ = head_;

    ++size_;
}

template <typename T>
void LinkedList<T>::PushBack(const T& item)
{
    auto new_node = new Node{ item, nullptr };
    if (tail_) tail_->next_ = new_node;
    tail_ = new_node;
    if (!head_) head_ = tail_;

    ++size_;
}

template <typename T>
void LinkedList<T>::PopFront()
{
    auto old_head = head_;
    head_ = head_->next_;
    delete old_head;

    if (!head_) tail_ = nullptr;

    --size_;
}




template <typename T>
void LinkedList<T>::Insert(Node* dest, const T& item)
{

}

template <typename T>
void LinkedList<T>::Delete(Node* node)
{

}

template <typename T>
void LinkedList<T>::Clear()
{
    Node* prev = nullptr;
    Node* curr = head_;

    while (curr)
    {
        prev = curr;
        curr = curr->next_;

        delete prev;
    }
}




template <typename T>
T& LinkedList<T>::Front()
{
    return head_->val_;
}

template <typename T>
const T& LinkedList<T>::Front() const
{
    return head_->val_;
}

template <typename T>
T& LinkedList<T>::Back()
{
    return tail_->val_;
}

template <typename T>
const T& LinkedList<T>::Back() const
{
    return tail_->val_;
}

template <typename T>
auto LinkedList<T>::Head() -> Node*
{
    return head_;
}

template <typename T>
auto LinkedList<T>::Head() const -> const Node*
{
    return head_;
}




template <typename T>
void LinkedList<T>::Reverse()
{

}


template <typename T>
void LinkedList<T>::Sort()
{

}
