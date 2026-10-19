#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

// Linked LIFO stack. std::unique_ptr here owns nodes; the stack behavior is ours.
template<class T> class MyStack {
    struct Node {T value;std::unique_ptr<Node> next;explicit Node(const T& v):value(v){} };
    std::unique_ptr<Node> top_;
    int count_=0;
    static std::unique_ptr<Node> copyNodes(const std::unique_ptr<Node>& src){
        if(!src)return nullptr;
        auto result=std::make_unique<Node>(src->value);
        result->next=copyNodes(src->next);
        return result;
    }
public:
    MyStack()=default;
    MyStack(const MyStack& other):top_(copyNodes(other.top_)),count_(other.count_){}
    MyStack& operator=(const MyStack& other){if(this!=&other){MyStack copy(other);swap(copy);}return *this;}
    void swap(MyStack& other)noexcept{top_.swap(other.top_);std::swap(count_,other.count_);}
    bool empty()const{return !top_;}
    int size()const{return count_;}
    void push(const T& value){auto node=std::make_unique<Node>(value);node->next=std::move(top_);top_=std::move(node);++count_;}
    T pop(){if(empty())throw std::out_of_range("stack is empty");T item=top_->value;top_=std::move(top_->next);--count_;return item;}
    void clear(){while(top_)top_=std::move(top_->next);count_=0;}
};
