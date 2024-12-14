#pragma once
#include "GPS.h"
#include "IComparable.h"

using namespace std;
class Model {
public:
    virtual ~Model() = default;

    virtual IPrototype* getKey() const = 0;
};

class Nehnutelnost : public IComparable<Nehnutelnost>, public IPrototype, public Model
{
private:
    int uid;
    GPS *gps;
    int supisneCislo;
    std::string popis;

public:
    Nehnutelnost(int id, GPS *gpsCoord, int supisne = -1, const std::string &desc = "")
        : uid(id)
        , gps(new GPS(*gpsCoord))
        , supisneCislo(supisne)
        , popis(desc)
    {}

    Nehnutelnost(const Nehnutelnost &other)
        : uid(other.uid)
        , gps(new GPS(*other.gps))
        , supisneCislo(other.supisneCislo)
        , popis(other.popis)
    {}

    ~Nehnutelnost()
    {
        if (gps) {
            delete gps;
            gps = nullptr;
        }
    }

    bool equals(const Nehnutelnost &other) const override
    {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Nehnutelnost &other) const override
    {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Nehnutelnost &other, int cur_level) const override
    {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream &operator<<(std::ostream &os, const Nehnutelnost &nehnutelnost)
    {
        os << "Nehnutelnost(uid: " << nehnutelnost.uid << ", GPS: " << *nehnutelnost.gps
           << ", Supisne cislo: "
           << (nehnutelnost.supisneCislo == -1 ? "N/A" : std::to_string(nehnutelnost.supisneCislo))
           << ", Popis: " << (nehnutelnost.popis.empty() ? "No description" : nehnutelnost.popis)
           << ")";
        return os;
    }

    int getUid() const { return uid; }
    int getSupisneCislo() const { return supisneCislo; }
    const std::string &getPopis() const { return popis; }

    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS *newGps) { gps = newGps; }
    void setSupisneCislo(int newSupisneCislo) { supisneCislo = newSupisneCislo; }
    void setPopis(const std::string &newPopis) { popis = newPopis; }



    IPrototype *clone() override {
        return new Nehnutelnost(*this);
    };

    IPrototype *getKey() const override {
        return this->gps;
    }
};

\

class Parcela : public IComparable<Parcela>, public IPrototype, public Model
{
private:
    int uid;
    GPS *gps;
    int cisloParcely;
    std::string popis;

public:
    Parcela(int id, GPS *gpsCoord, int cislo = -1, const std::string &desc = "")
        : uid(id)
        , gps(new GPS(*gpsCoord))
        , cisloParcely(cislo)
        , popis(desc)
    {}

    Parcela(const Parcela &other)
        : uid(other.uid)
        , gps(new GPS(*other.gps))
        , cisloParcely(other.cisloParcely)
        , popis(other.popis)
    {}

    ~Parcela()
    {
        clearNehnutelnosti();
        if (gps) {
            delete gps;
            gps = nullptr;
        }
    }

    bool equals(const Parcela &other) const override
    {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Parcela &other) const override
    {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Parcela &other, int cur_level) const override
    {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream &operator<<(std::ostream &os, const Parcela &parcela)
    {
        os << "Parcela(uid: " << parcela.uid << ", GPS: " << *parcela.gps << ", Cislo parcely: "
           << (parcela.cisloParcely == -1 ? "N/A" : std::to_string(parcela.cisloParcely))
           << ", Popis: " << (parcela.popis.empty() ? "No description" : parcela.popis) << ")";
        return os;
    }

    int getUid() const { return uid; }
    int getCisloParcely() const { return cisloParcely; }
    const std::string &getPopis() const { return popis; }

    void setUid(int newUid) { uid = newUid; }
    void setGps(GPS *newGps) { gps = newGps; }
    void setCisloParcely(int newCisloParcely) { cisloParcely = newCisloParcely; }
    void setPopis(const std::string &newPopis) { popis = newPopis; }

    void removeNehnutelnost(Nehnutelnost *nehnut);
    void clearNehnutelnosti();

    IPrototype *clone() override {
        return this->gps;
    }

    IPrototype *getKey() const override {
        return this->gps;
    }
};
