#include "DynamicRecords.h"
DynamicRecords::DynamicRecords() : DynamicRecords(2) {}
DynamicRecords::DynamicRecords(int startingCapacity) : items_(nullptr), size_(0), capacity_(startingCapacity>0 ? startingCapacity : 1) { items_=new std::string[capacity_]; }
DynamicRecords::DynamicRecords(const DynamicRecords& other) : items_(new std::string[other.capacity_]),size_(other.size_),capacity_(other.capacity_) {
    for(int i=0;i<size_;++i) items_[i]=other.items_[i];
}
DynamicRecords& DynamicRecords::operator=(const DynamicRecords& other) {
    if(this!=&other) { DynamicRecords copy(other); std::swap(items_,copy.items_);std::swap(size_,copy.size_);std::swap(capacity_,copy.capacity_); }
    return *this;
}
DynamicRecords::~DynamicRecords(){delete[] items_;}
void DynamicRecords::grow(){
    int newCap=capacity_*2;
    std::string* bigger=new std::string[newCap];
    for(int i=0;i<size_;++i) bigger[i]=items_[i];
    delete[] items_;items_=bigger;capacity_=newCap;
}
void DynamicRecords::add(const std::string& entry){if(size_==capacity_)grow();items_[size_++]=entry;}
const std::string& DynamicRecords::at(int index) const {if(index<0||index>=size_)throw std::out_of_range("record index");return items_[index];}
void DynamicRecords::replace(int index,const std::string& entry){if(index<0||index>=size_)throw std::out_of_range("record index");items_[index]=entry;}
