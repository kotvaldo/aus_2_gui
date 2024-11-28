#pragma once

#include "GPS.h"
#include "IComparable.h"
#include "Nehnutelnost.h"
#include "Parcela.h"

class Area : public IComparable<Area>, public IPrototype
{
private:
    int uid;
    GPS *gps;
    Nehnutelnost *nehnutelnost;
    Parcela *parcela;

public:
    Area(int id, GPS *gpsCoord, Nehnutelnost *nehnut = nullptr, Parcela *parc = nullptr)
        : uid(id)
        , gps(new GPS(*gpsCoord))
        , nehnutelnost(nehnut)
        , parcela(parc)
    {}

    Area(const Area &other)
        : uid(other.uid)
        , gps(new GPS(*other.gps))
        , nehnutelnost(other.nehnutelnost ? new Nehnutelnost(*other.nehnutelnost) : nullptr)
        , parcela(other.parcela ? new Parcela(*other.parcela) : nullptr)
    {}

    ~Area()
    {
        std::cout << "Deleting Area: " << this << ", GPS: " << gps << std::endl;

        if (gps) {
            delete gps;
            gps = nullptr;
            nehnutelnost = nullptr;
            parcela = nullptr;
        }
    }

    bool equals(const Area &other) const override
    {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Area &other) const override
    {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Area &other, int cur_level) const override
    {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream &operator<<(std::ostream &os, const Area &area)
    {
        os << "Area(uid: " << area.uid << ", GPS: " << *area.gps;
        if (area.nehnutelnost)
            os << ", Nehnutelnost: " << *area.nehnutelnost;
        if (area.parcela)
            os << ", Parcela: " << *area.parcela;
        os << ")";
        return os;
    }

    Area* clone() override {
        return new Area(*this);
    }

    // Getters
    int getUid() const { return uid; }
    GPS *getGps() const { return gps; }
    Nehnutelnost *getNehnutelnost() const { return nehnutelnost; }
    Parcela *getParcela() const { return parcela; }
    void setNehnutelnost(Nehnutelnost *newNehnutelnost) { nehnutelnost = newNehnutelnost; }
    void setParcela(Parcela *newParcela) { parcela = newParcela; }

};

