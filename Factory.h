#pragma once
#include "GPS.h"
#include "IPrototype.h"
#include "Model.h"
#include "Parameters.h"
#include <memory>


class IModelFactory {
public:
    virtual std::shared_ptr<IPrototype> createInstance(Parameters keyParameters, Parameters dataParameters) = 0;
    virtual std::shared_ptr<IPrototype> createInstance(Parameters keyParameters) = 0;
};



class NehnutelnostFactory : public IModelFactory {
public:
    NehnutelnostFactory() = default;

    std::shared_ptr<IPrototype> createInstance(Parameters keyParameters, Parameters dataParameters) override {
        try {
            GPSParameters& keyParams = dynamic_cast<GPSParameters&>(keyParameters);
            ObjParameters& dataParams = dynamic_cast<ObjParameters&>(dataParameters);

            std::shared_ptr<GPS> gps = std::make_shared<GPS>(keyParams.x, keyParams.y, keyParams.width, keyParams.length);

            return std::make_shared<Nehnutelnost>(dataParams.number, gps, dataParams.number, dataParams.description);

        } catch (const std::bad_cast& e) {
            std::cerr << "Error during casting: " << e.what() << std::endl;
            return nullptr;
        }
    }
};



class ParcelaFactory : public IModelFactory {
public:
    ParcelaFactory() = default;

    std::shared_ptr<IPrototype> createInstance(Parameters keyParameters, Parameters dataParameters) override {
        try {
            ObjParameters& dataParams = dynamic_cast<ObjParameters&>(dataParameters);

            GPSParameters& keyParams = dynamic_cast<GPSParameters&>(keyParameters);
            std::shared_ptr<GPS> gps = std::make_shared<GPS>(keyParams.x, keyParams.y, keyParams.width, keyParams.length);

            return std::make_shared<Parcela>(dataParams.number, gps, dataParams.number, dataParams.description);

        } catch (const std::bad_cast& e) {
            std::cerr << "Error during casting: " << e.what() << std::endl;
            return nullptr;
        }
    }
};

class GPSFactory : public IModelFactory {
public:
    GPSFactory() = default;

    std::shared_ptr<IPrototype> createInstance(Parameters keyParameters) override {
        try {
            GPSParameters& keyParams = dynamic_cast<GPSParameters&>(keyParameters);

            return std::make_shared<GPS>(keyParams.x, keyParams.y, keyParams.width, keyParams.length);

        } catch (const std::bad_cast& e) {
            std::cerr << "Error during casting: " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPrototype> createInstance(Parameters keyParameters, Parameters dataParameters) override {
        return nullptr;
    }
};
