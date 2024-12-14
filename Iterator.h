#pragma once

#include "IPrototype.h"
#include "Node.h"


class BaseIterator {
public:
    virtual ~BaseIterator() = default;

    virtual void operator++() = 0;
    virtual bool operator!=(const BaseIterator& other) const = 0;
    virtual IPrototype* operator*() const = 0;
};



