#include "GPS.h"

class Parcela : public IComparable<Parcela>, public IPrototype {
private:
    int uid;
    GPS *gps;
    int cisloParcely;
    std::string popis;

public:
    Parcela(int id, GPS *gpsCoord, int cislo = -1, const std::string &desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), cisloParcely(cislo), popis(desc) {}

    Parcela(const Parcela& other)
        : uid(other.uid), gps(new GPS(*other.gps)), cisloParcely(other.cisloParcely), popis(other.popis) {}
    ~Parcela() {
        if (gps) delete gps;
    }
    bool equals(const Parcela& other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Parcela& other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Parcela& other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream &operator<<(std::ostream &os, const Parcela &parcela)
    {
        os << "Parcela(uid: " << parcela.uid << ", GPS: " << *parcela.gps
           << ", Cislo parcely: " << parcela.cisloParcely
           << ", Popis: " << (parcela.popis.empty() ? "N/A" : parcela.popis);
        os << ")";
        return os;
    }

    // Getters
    int getUid() const { return uid; }
    GPS* getGps() const { return gps; }
    int getCisloParcely() const { return cisloParcely; }
    const std::string& getPopis() const { return popis; }

    // Setters
    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS *newGps) { gps = newGps; }
    void setCisloParcely(int newCisloParcely) { cisloParcely = newCisloParcely; }
    void setPopis(const std::string &newPopis) { popis = newPopis; }

    IPrototype* clone() override {
        return new Parcela(*this);
    }
};
