#pragma once
template<typename T>
class IComparable {
public:
    virtual int compare(const T& other, int cur_level) const = 0;
    virtual bool equals(const T& other) const = 0;
    virtual bool equalsByKeys(const T& other) const = 0;
};
