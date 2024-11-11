#pragma once
#include "GPS.h"
#include "IComparable.h"

#include <vector>

class Nehnutelnost;  // Dopredná deklarácia triedy Parcela

class Parcela : public IComparable<Parcela> {
private:
    int uid;
    GPS* gps;
    int cisloParcely;
    std::string popis;
    std::vector<Nehnutelnost*> nehnutelnosti;

public:
    Parcela(int id, GPS* gpsCoord, int cislo = -1, const std::string& desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), cisloParcely(cislo), popis(desc) {}

    Parcela(const Parcela& other)
        : uid(other.uid), gps(new GPS(*other.gps)), cisloParcely(other.cisloParcely), popis(other.popis), nehnutelnosti(other.nehnutelnosti) {}

    ~Parcela() { delete gps; }

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
        os << "Parcela(uid: " << parcela.uid
           << ", GPS: " << *parcela.gps
           << ", Cislo parcely: " << (parcela.cisloParcely == -1 ? "N/A" : std::to_string(parcela.cisloParcely))
           << ", Popis: " << (parcela.popis.empty() ? "No description" : parcela.popis) << ")";
        return os;
    }


    int getUid() const { return uid; }
    GPS* getGps() const { return gps; }
    int getCisloParcely() const { return cisloParcely; }
    const std::string& getPopis() const { return popis; }
    const std::vector<Nehnutelnost*>& getNehnutelnosti() const { return nehnutelnosti; }

    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS* newGps) { if (gps) delete gps; gps = new GPS(*newGps); }
    void setCisloParcely(int newCisloParcely) { cisloParcely = newCisloParcely; }
    void setPopis(const std::string& newPopis) { popis = newPopis; }
    void addNehnutelnost(Nehnutelnost* nehnut) { nehnutelnosti.push_back(nehnut); }

    void removeNehnutelnost(Nehnutelnost *nehnut);
    void clearNehnutelnosti();
};


