#pragma once

#include "GPS.h"
#include "IPrototype.h"
#include "Models.h"
#include "Util.h"
#include <vector>
using namespace std;



class Generator {
public:
    virtual IPrototype* generateRandomInstance() = 0;
    virtual IPrototype* generateRandomKey() = 0;
    virtual IPrototype* generateInstanceWithSameKey() = 0;
    virtual ~Generator() {
        keys.clear();
    }
    void storeKey(IPrototype* key) { keys.push_back(key); }
protected:
     std::vector<IPrototype*> keys;

};

class NehnutelnostGenerator : public Generator {
public:
    static NehnutelnostGenerator& getInstance() {
        static NehnutelnostGenerator instance;
        return instance;
    }

    IPrototype* generateRandomInstance() override {
        GPS* key = dynamic_cast<GPS*>(generateRandomKey());
        int supisneCislo = Util::randomNumber(1, 1000);
        std::string description = Util::randomString(10);
        return new Nehnutelnost(Util::randomNumber(1, 1000), key, supisneCislo, description);
    }

    IPrototype* generateRandomKey() override {
        double x = Util::randomNumber(0.0, 100.0);
        double y = Util::randomNumber(0.0, 100.0);
        char width = Util::randomChar({'A', 'B', 'C', 'D'});
        char length = Util::randomChar({'E', 'F', 'G', 'H'});

        GPS* key = new GPS(x, y, width, length);
        storeKey(key);
        return key;
    }

    IPrototype* generateInstanceWithSameKey() override {
        if (keys.empty()) {
            throw std::runtime_error("No keys available to clone.");
        }

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, keys.size() - 1);
        size_t randomIndex = dist(rng);

        GPS* clonedKey = dynamic_cast<GPS*>(keys[randomIndex]->clone());
        return new Nehnutelnost(Util::randomNumber(1, 1000), clonedKey, Util::randomNumber(1, 1000), "SameKeyInstance");
    }

private:
    NehnutelnostGenerator() = default;
    ~NehnutelnostGenerator() = default;

    NehnutelnostGenerator(const NehnutelnostGenerator&) = delete;
    NehnutelnostGenerator& operator=(const NehnutelnostGenerator&) = delete;
};



class ParcelaGenerator : public Generator {
public:
    static ParcelaGenerator& getInstance() {
        static ParcelaGenerator instance;
        return instance;
    }

    IPrototype* generateRandomInstance() override {
        GPS* key = dynamic_cast<GPS*>(generateRandomKey());
        int cisloParcely = Util::randomNumber(1, 500);
        std::string description = Util::randomString(15);

        return new Parcela(Util::randomNumber(1, 1000), key, cisloParcely, description);
    }

    IPrototype* generateRandomKey() override {
        double x = Util::randomNumber(0.0, 100.0);
        double y = Util::randomNumber(0.0, 100.0);
        char width = Util::randomChar({'A', 'B', 'C', 'D'});
        char length = Util::randomChar({'E', 'F', 'G', 'H'});

        GPS* key = new GPS(x, y, width, length);
        storeKey(key);
        return key;
    }

    IPrototype* generateInstanceWithSameKey() override {
        if (keys.empty()) {
            throw std::runtime_error("No keys available to clone.");
        }

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, keys.size() - 1);
        size_t randomIndex = dist(rng);

        GPS* clonedKey = dynamic_cast<GPS*>(keys[randomIndex]->clone());
        return new Parcela(Util::randomNumber(1, 1000), clonedKey, Util::randomNumber(1, 500), "SameKeyParcela");
    }

private:
    ParcelaGenerator() = default;
    ~ParcelaGenerator() = default;

    ParcelaGenerator(const ParcelaGenerator&) = delete;
    ParcelaGenerator& operator=(const ParcelaGenerator&) = delete;
};

