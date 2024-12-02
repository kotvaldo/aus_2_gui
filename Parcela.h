#pragma once
#include <memory>
#include <iostream>
#include "GPS.h"
#include "Model.h"

class Parcela : public IComparable<Parcela>, public IPrototype, public Model<GPS> {
private:
    int uid;
    std::shared_ptr<GPS> gps;
    int cisloParcely;
    std::string popis;

public:
    Parcela(int id, std::shared_ptr<GPS> gpsCoord, int cislo = -1, const std::string& desc = "")
        : uid(id), gps(gpsCoord), cisloParcely(cislo), popis(desc) {}

    Parcela(const Parcela& other)
        : uid(other.uid), gps(std::make_shared<GPS>(*other.gps)), cisloParcely(other.cisloParcely), popis(other.popis) {}

    ~Parcela() = default;

    bool equals(const Parcela& other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Parcela& other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Parcela& other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream& operator<<(std::ostream& os, const Parcela& parcela) {
        os << "Parcela(uid: " << parcela.uid << ", GPS: " << *parcela.gps
           << ", Cislo parcely: " << parcela.cisloParcely
           << ", Popis: " << (parcela.popis.empty() ? "N/A" : parcela.popis) << ")";
        return os;
    }


    int getUid() const { return uid; }
    std::shared_ptr<GPS> getGps() const { return gps; }
    int getCisloParcely() const { return cisloParcely; }
    const std::string& getPopis() const { return popis; }

    void setUid(int newUid) { uid = newUid; }
    void setCisloParcely(int newCisloParcely) { cisloParcely = newCisloParcely; }
    void setPopis(const std::string& newPopis) { popis = newPopis; }


    std::shared_ptr<IPrototype> clone() override {
        return std::make_shared<Parcela>(*this);
    }

    std::shared_ptr<GPS> getKey() override {
        return this->gps;
    }
};
