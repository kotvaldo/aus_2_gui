#pragma once
#include "Builders.h"
#include "Parameters.h"
#include "Prototype.h"

class IFactory {
    virtual IPrototype* createInstance() = 0;
};

class NehnutelnostFactory : public IFactory {
    IPrototype* createInstance(GPSParameters gpsCoords, ObjParameters params) {
        return
    }
};

class ParcelaFactory : public IFactory {

};

class AreaFactory : public IFactory {

};
