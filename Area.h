#pragma once
#include <memory>
#include <iostream>
#include "GPS.h"
#include "IComparable.h"
#include "Nehnutelnost.h"
#include "Parcela.h"

class Area : public IComparable<Area>, public IPrototype {
private:
    int uid;
    std::shared_ptr<GPS> gps;
    std::shared_ptr<Nehnutelnost> nehnutelnost;
    std::shared_ptr<Parcela> parcela;

public:
    Area(int id, std::shared_ptr<GPS> gpsCoord, std::shared_ptr<Nehnutelnost> nehnut = nullptr, std::shared_ptr<Parcela> parc = nullptr)
        : uid(id)
        , gps(std::move(gpsCoord))
        , nehnutelnost(std::move(nehnut))
        , parcela(std::move(parc)) {}

    Area(const Area &other)
        : uid(other.uid)
        , gps(std::make_shared<GPS>(*other.gps)) // Vytvoríme novú kópiu GPS
        , nehnutelnost(other.nehnutelnost ? std::make_shared<Nehnutelnost>(*other.nehnutelnost) : nullptr)
        , parcela(other.parcela ? std::make_shared<Parcela>(*other.parcela) : nullptr) {}

    virtual ~Area() {
        std::cout << "Deleting Area: " << this << ", GPS: " << gps.get() << std::endl;
    }

    bool equals(const Area &other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Area &other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Area &other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }


    friend std::ostream &operator<<(std::ostream &os, const Area &area) {
        os << "Area(uid: " << area.uid << ", GPS: " << *area.gps;
        if (area.nehnutelnost)
            os << ", Nehnutelnost: " << *area.nehnutelnost;
        if (area.parcela)
            os << ", Parcela: " << *area.parcela;
        os << ")";
        return os;
    }

    // Klonovanie (Prototype Pattern)
    Area *clone() override {
        return new Area(*this);
    }

    // Gettery
    int getUid() const { return uid; }
    std::shared_ptr<GPS> getGps() const { return gps; }
    std::shared_ptr<Nehnutelnost> getNehnutelnost() const { return nehnutelnost; }
    std::shared_ptr<Parcela> getParcela() const { return parcela; }

    // Settery
    void setNehnutelnost(std::shared_ptr<Nehnutelnost> newNehnutelnost) {
        nehnutelnost = std::move(newNehnutelnost);
    }

    void setParcela(std::shared_ptr<Parcela> newParcela) {
        parcela = std::move(newParcela);
    }
};
