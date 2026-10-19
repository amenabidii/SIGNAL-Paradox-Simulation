#pragma once
#include <stdexcept>
#include <utility>
#include <vector>

// MyQueue is a linked FIFO queue, not a wrapper around std::queue.
template<class T> class MyQueue {
    struct Node {T value;Node* next=nullptr;explicit Node(const T& x):value(x){} };
    Node* first_=nullptr;
    Node* last_=nullptr;
    int count_=0;
public:
    MyQueue()=default;
    MyQueue(const MyQueue& other){for(const auto& x:other.values())enqueue(x);}
    MyQueue& operator=(const MyQueue& other){if(this!=&other){MyQueue copy(other);swap(copy);}return *this;}
    ~MyQueue(){clear();}
    void swap(MyQueue& other)noexcept{std::swap(first_,other.first_);std::swap(last_,other.last_);std::swap(count_,other.count_);}
    bool empty()const{return !first_;}
    int size()const{return count_;}
    void enqueue(const T& value){Node* n=new Node(value);if(last_)last_->next=n;else first_=n;last_=n;++count_;}
    T dequeue(){if(empty())throw std::out_of_range("queue is empty");Node* old=first_;T result=old->value;first_=first_->next;if(!first_)last_=nullptr;delete old;--count_;return result;}
    std::vector<T> values()const{std::vector<T> result;for(Node* n=first_;n;n=n->next)result.push_back(n->value);return result;}
    void clear(){while(first_){Node* n=first_;first_=first_->next;delete n;}last_=nullptr;count_=0;}
};
