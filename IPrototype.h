#pragma once

class IPrototype {
public:
    virtual ~IPrototype() = default;
    virtual IPrototype* clone() = 0;
};
