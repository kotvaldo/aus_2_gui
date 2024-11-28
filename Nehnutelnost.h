#include "GPS.h"

class Nehnutelnost : public IComparable<Nehnutelnost>, public IPrototype {
private:
    int uid;
    GPS *gps;
    int supisneCislo;
    std::string popis;

public:
    Nehnutelnost(int id, GPS *gpsCoord, int supisne = -1, const std::string &desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), supisneCislo(supisne), popis(desc) {}

    Nehnutelnost(const Nehnutelnost& other)
        : uid(other.uid), gps(new GPS(*other.gps)), supisneCislo(other.supisneCislo), popis(other.popis) {}

    ~Nehnutelnost() {
        if (gps) delete gps;
    }

    bool equals(const Nehnutelnost& other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Nehnutelnost& other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Nehnutelnost& other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream &operator<<(std::ostream &os, const Nehnutelnost &nehnutelnost)
    {
        os << "Nehnutelnost(uid: " << nehnutelnost.uid << ", GPS: " << *nehnutelnost.gps
           << ", Supisne cislo: " << nehnutelnost.supisneCislo
           << ", Popis: " << (nehnutelnost.popis.empty() ? "N/A" : nehnutelnost.popis);
        os << ")";
        return os;
    }


    // Getters
    int getUid() const { return uid; }
    GPS* getGps() const { return gps; }
    int getSupisneCislo() const { return supisneCislo; }
    const std::string& getPopis() const { return popis; }

    // Setters
    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS *newGps) { gps = newGps; }
    void setSupisneCislo(int newSupisneCislo) { supisneCislo = newSupisneCislo; }
    void setPopis(const std::string &newPopis) { popis = newPopis; }

    IPrototype* clone() override {
        return new Nehnutelnost(*this);
    }
};
