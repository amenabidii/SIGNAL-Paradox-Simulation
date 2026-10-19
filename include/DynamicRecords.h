#pragma once
#include <string>
#include <stdexcept>
#include <utility>

// Each researcher keeps their own resizable record array. We used a separate class
// so the six researcher roles do not repeat the same new[] / delete[] logic.
class DynamicRecords {
    std::string* items_;
    int size_;
    int capacity_;
    void grow();
public:
    DynamicRecords();
    explicit DynamicRecords(int startingCapacity);
    DynamicRecords(const DynamicRecords& other);
    DynamicRecords& operator=(const DynamicRecords& other);
    ~DynamicRecords();
    void add(const std::string& entry);
    int size() const { return size_; }
    const std::string& at(int index) const;
    void replace(int index, const std::string& entry);
};
