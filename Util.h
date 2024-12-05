#pragma once

#include "IPrototype.h"
#include "Factory.h"

#include <iostream>
#include <memory>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

class Util {
private:
    static std::random_device rd;
    static std::mt19937 rng;
    static std::vector<int> idGenerators;
public:
    template <typename T>
    static T randomNumber(T min, T max) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(rng);
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(rng);
        }
    }

    static std::string randomString(int length) {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::uniform_int_distribution<> dist(0, chars.size() - 1);
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += chars[dist(rng)];
        }
        return result;
    }

    static int CompareDoubles(double value1, double value2) {
        if (value1 < value2) return -1;
        if (value1 > value2) return 1;
        return 0;
    }

    static int CompareStrings(const std::string& value1, const std::string& value2) {
        if (value1 < value2) return -1;
        if (value1 > value2) return 1;
        return 0;
    }

    static std::string FormatDoubleForExport(double number) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << number;
        return ss.str();
    }

    static std::string FormatDoubleForImport(const std::string& number) {
        std::string result = number;
        std::replace(result.begin(), result.end(), '.', ',');
        return result;
    }


    template <typename TFactory, typename TParameters>
    static std::shared_ptr<IPrototype> generateKey(
        ) {
        static_assert(std::is_base_of<IModelFactory, TFactory>::value, "TFactory must derive from IModelFactory");
        static_assert(std::is_base_of<Parameters, TParameters>::value, "TParameters must derive from Parameters");

        auto parameters = std::make_shared<TParameters>();
        auto factory = std::make_shared<TFactory>();
        if (!factory || !parameters) {
            std::cerr << "Factory or Parameters are null!" << std::endl;
            return nullptr;
        }

        parameters->randomize();

        return factory->createInstance(parameters);
    }

    template <typename KeyFactory, typename KeyParameters, typename DataFactory, typename DataParameters>
    static std::shared_ptr<IPrototype> generateRandomData() {
        auto key = generateKey<KeyFactory, KeyParameters>();
        if (!key) {
            std::cerr << "Failed to generate key!" << std::endl;
            return nullptr;
        }

        auto dataParameters = std::make_shared<DataParameters>();
        if (!dataParameters) {
            std::cerr << "DataParameters are null!" << std::endl;
            return nullptr;
        }

        dataParameters->randomize();

        auto dataFactory = std::make_shared<DataFactory>();
        if (!dataFactory) {
            std::cerr << "DataFactory is null!" << std::endl;
            return nullptr;
        }

        return dataFactory->createInstance(*key, *dataParameters);
    }

    template <typename DataFactory, typename DataParameters, typename Key>
    static std::shared_ptr<IPrototype> duplicateKeyAndRandomizeData(
        const std::shared_ptr<Key>& existingKey) {
        if (!existingKey) {
            std::cerr << "Provided key is null!" << std::endl;
            return nullptr;
        }

        auto clonedKey = existingKey->clone();
        if (!clonedKey) {
            std::cerr << "Failed to clone the key!" << std::endl;
            return nullptr;
        }

        auto dataParameters = std::make_shared<DataParameters>();
        if (!dataParameters) {
            std::cerr << "DataParameters are null!" << std::endl;
            return nullptr;
        }
        dataParameters->randomize();

        auto dataFactory = std::make_shared<DataFactory>();
        if (!dataFactory) {
            std::cerr << "DataFactory is null!" << std::endl;
            return nullptr;
        }

        return dataFactory->createInstance(*clonedKey, *dataParameters);
    }


};


std::random_device Util::rd;
std::mt19937 Util::rng(Util::rd());
