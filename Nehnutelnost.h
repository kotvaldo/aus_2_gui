#pragma once
#include "GPS.h"
#include "IComparable.h"
#include "Parcela.h"
#include <vector>
using namespace std;



class Nehnutelnost : public IComparable<Nehnutelnost> {
private:
    int uid;
    GPS* gps;
    int supisneCislo;
    std::string popis;
    std::vector<Parcela*> parcely;

public:
    Nehnutelnost(int id, GPS* gpsCoord, int supisne = -1, const std::string& desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), supisneCislo(supisne), popis(desc) {}

    Nehnutelnost(const Nehnutelnost& other)
        : uid(other.uid), gps(new GPS(*other.gps)), supisneCislo(other.supisneCislo), popis(other.popis), parcely(other.parcely) {}

    ~Nehnutelnost() { delete gps; }

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
        os << "Nehnutelnost(uid: " << nehnutelnost.uid
           << ", GPS: " << *nehnutelnost.gps
           << ", Supisne cislo: " << (nehnutelnost.supisneCislo == -1 ? "N/A" : std::to_string(nehnutelnost.supisneCislo))
           << ", Popis: " << (nehnutelnost.popis.empty() ? "No description" : nehnutelnost.popis) << ")";
        return os;
    }

    // Getters
    int getUid() const { return uid; }
    GPS* getGps() const { return gps; }
    int getSupisneCislo() const { return supisneCislo; }
    const std::string& getPopis() const { return popis; }
    const std::vector<Parcela*>& getParcely() const { return parcely; }

    // Setters
    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS* newGps) { if (gps) delete gps; gps = new GPS(*newGps); }
    void setSupisneCislo(int newSupisneCislo) { supisneCislo = newSupisneCislo; }
    void setPopis(const std::string& newPopis) { popis = newPopis; }
    void addParcela(Parcela* parcela) { parcely.push_back(parcela); }

    void clearParcely();
    void removeParcela(Parcela *parcela);
};



