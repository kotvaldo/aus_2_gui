#pragma once
#include <memory>

class IPrototype {
public:
    virtual ~IPrototype() = default;

    virtual std::shared_ptr<IPrototype> clone() = 0;
};
