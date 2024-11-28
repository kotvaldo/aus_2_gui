#pragma once
#include "Parameters.h"


class ParcelaBuilder {
    int supisneCislo_ = -1;
    std::string popis_ = "";
};

class NehnutelnostBuilder {
public:
    NehnutelnostBuilder& uid(int value) {
        uid_ = value;
        return *this;
    }

    NehnutelnostBuilder& gps(GPSParameters gpsCoords) {
        gps_ =
        return *this;
    }

    NehnutelnostBuilder& supisneCislo(int value) {
        supisneCislo_ = value;
        return *this;
    }

    NehnutelnostBuilder& popis(const std::string& value) {
        popis_ = value;
        return *this;
    }

    Nehnutelnost build() const {
        return Nehnutelnost(uid_, gps_.get(), supisneCislo_, popis_);
    }

private:
    int uid_ = -1;
    std::unique_ptr<GPS> gps_ = nullptr;
    int supisneCislo_ = -1;
    std::string popis_ = "";
};


class AreaBuilder {
public:


private:
    // tu bude urcite jeden nullptr;

};
