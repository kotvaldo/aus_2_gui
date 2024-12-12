#pragma once
#include "IPrototype.h"
#include "Parameters.h"


class IModelFactory {
public:
    virtual IPrototype* createInstance(Parameters keyParameters, Parameters dataParameters) = 0;
    virtual IPrototype* createInstance(Parameters keyParameters) = 0;

    virtual Generator* createGenerator();
    virtual Comparator* createComparator();
    virtual FileLoader* createFileLoader();
};


