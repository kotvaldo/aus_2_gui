#pragma once
#include "FileLoader.h"
#include "Generator.h"
#include "IPrototype.h"
#include "Models.h"
#include "Parameters.h"

class IModelFactory {
public:
    virtual IPrototype* createInstance(Parameters keyParameters, Parameters dataParameters) = 0;
    virtual IPrototype* createInstance(Parameters keyParameters) = 0;

    virtual Generator* createGenerator();;
    virtual BaseFileLoader* createFileLoader();
};

class NehnutelnostFactory : public IModelFactory {
public:
    NehnutelnostFactory() = default;

    IPrototype* createInstance(Parameters keyParameters, Parameters dataParameters) override {
        try {
            GPSParameters& keyParams = dynamic_cast<GPSParameters&>(keyParameters);
            ObjParameters& dataParams = dynamic_cast<ObjParameters&>(dataParameters);

            GPS* gps = new GPS(keyParams.x, keyParams.y, keyParams.width, keyParams.length);

            return new Nehnutelnost(dataParams.number, gps, dataParams.number, dataParams.description);

        } catch (const std::bad_cast& e) {
            std::cerr << "Error during casting: " << e.what() << std::endl;
            return nullptr;
        }
    }

    Generator* createGenerator() override {
        return &NehnutelnostGenerator::getInstance();
    }
    BaseFileLoader* createFileLoader() override {
        return &NehnutelnostFileLoader::getInstance();
    }


};

class ParcelaFactory : public IModelFactory {
public:
    ParcelaFactory() = default;

    IPrototype* createInstance(Parameters keyParameters, Parameters dataParameters) override {
        try {
            ObjParameters& dataParams = dynamic_cast<ObjParameters&>(dataParameters);
            GPSParameters& keyParams = dynamic_cast<GPSParameters&>(keyParameters);

            GPS* gps = new GPS(keyParams.x, keyParams.y, keyParams.width, keyParams.length);

            return new Parcela(dataParams.number, gps, dataParams.number, dataParams.description);

        } catch (const std::bad_cast& e) {
            std::cerr << "Error during casting: " << e.what() << std::endl;
            return nullptr;
        }
    }

    Generator* createGenerator() override {
        return &ParcelaGenerator::getInstance();
    }
    BaseFileLoader* createFileLoader() override {
        return &ParcelaFileLoader::getInstance();
    }

};

