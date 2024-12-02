#pragma once
#include <memory>
#include "GPS.h"
#include "Model.h"

class Nehnutelnost : public IComparable<Nehnutelnost>, public IPrototype, public Model<GPS> {
private:
    int uid;
    std::shared_ptr<GPS> gps;
    int supisneCislo;
    std::string popis;

public:
    Nehnutelnost(int id, std::shared_ptr<GPS> gpsCoord, int supisne = -1, const std::string& desc = "")
        : uid(id), gps(gpsCoord), supisneCislo(supisne), popis(desc) {}

    Nehnutelnost(const Nehnutelnost& other)
        : uid(other.uid), gps(std::make_shared<GPS>(*other.gps)), supisneCislo(other.supisneCislo), popis(other.popis) {}

    ~Nehnutelnost() = default;

    bool equals(const Nehnutelnost& other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Nehnutelnost& other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Nehnutelnost& other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream& operator<<(std::ostream& os, const Nehnutelnost& nehnutelnost) {
        os << "Nehnutelnost(uid: " << nehnutelnost.uid << ", GPS: " << *nehnutelnost.gps
           << ", Supisne cislo: " << nehnutelnost.supisneCislo
           << ", Popis: " << (nehnutelnost.popis.empty() ? "N/A" : nehnutelnost.popis) << ")";
        return os;
    }

    int getUid() const { return uid; }

    int getSupisneCislo() const { return supisneCislo; }
    const std::string& getPopis() const { return popis; }

    void setSupisneCislo(int newSupisneCislo) { supisneCislo = newSupisneCislo; }
    void setPopis(const std::string& newPopis) { popis = newPopis; }

    std::shared_ptr<IPrototype> clone() override {
        return std::make_shared<Nehnutelnost>(*this);
    }

    std::shared_ptr<GPS> getKey() override {
        return this->gps;
    }

};
