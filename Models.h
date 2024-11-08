#pragma once
#include <iostream>
#include <string>

template<typename T>
class IComparable {
public:
    virtual int compare(const T& other, int cur_level) const = 0;
    virtual bool equals(const T& other) const = 0;
    virtual bool equalsByKeys(const T& other) const = 0;
    virtual ~IComparable() = default;
};

class GPS : public IComparable<GPS> {
public:
    double x;
    double y;
    char width;
    char length;

    GPS(double x = 0, double y = 0, char width = 'N', char length = 'W')
        : x(x), y(y), width(width), length(length) {
        std::cout << "GPS initialized: " << *this << std::endl;
    }

    GPS(const GPS& other)
        : x(other.x), y(other.y), width(other.width), length(other.length) {}

    int compare(const GPS& other, int cur_level) const override {
        if (cur_level % 4 == 0) {
            if (this->width < other.width) return -1;
            if (this->width > other.width) return 1;
        } else if (cur_level % 4 == 1) {
            if (this->x < other.x) return -1;
            if (this->x > other.x) return 1;
        } else if (cur_level % 4 == 2) {
            if (this->length < other.length) return -1;
            if (this->length > other.length) return 1;
        } else if (cur_level % 4 == 3) {
            if (this->y < other.y) return -1;
            if (this->y > other.y) return 1;
        }

        return 0;
    }

    bool equals(const GPS& other) const override {
        return this->x == other.x && this->y == other.y && this->width == other.width && this->length == other.length;
    }

    bool equalsByKeys(const GPS& other) const override {
        return this->x == other.x && this->y == other.y && this->width == other.width && this->length == other.length;
    }

    friend std::ostream& operator<<(std::ostream& os, const GPS& gps) {
        os << "(x: " << gps.x << ", y: " << gps.y
           << ", width: " << gps.width << ", length: " << gps.length << ")";
        return os;
    }
};

class Nehnutelnost : public IComparable<Nehnutelnost> {
public:
    int uid;
    GPS* gps;
    int supisneCislo;
    std::string popis;

    Nehnutelnost(int id, GPS* gpsCoord, int supisne = -1, const std::string& desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), supisneCislo(supisne), popis(desc) {}

    Nehnutelnost(const Nehnutelnost& other)
        : uid(other.uid), gps(new GPS(*other.gps)), supisneCislo(other.supisneCislo), popis(other.popis) {}

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
};

class Parcela : public IComparable<Parcela> {
public:
    int uid;
    GPS* gps;
    int cisloParcely;
    std::string popis;

    Parcela(int id, GPS* gpsCoord, int cislo = -1, const std::string& desc = "")
        : uid(id), gps(new GPS(*gpsCoord)), cisloParcely(cislo), popis(desc) {}

    Parcela(const Parcela& other)
        : uid(other.uid), gps(new GPS(*other.gps)), cisloParcely(other.cisloParcely), popis(other.popis) {}

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
};

class Area : public IComparable<Area> {
public:
    int uid;
    GPS* gps;
    Nehnutelnost* nehnutelnost;
    Parcela* parcela;

    Area(int id, GPS* gpsCoord, Nehnutelnost* nehnut = nullptr, Parcela* parc = nullptr)
        : uid(id), gps(new GPS(*gpsCoord)), nehnutelnost(nehnut), parcela(parc) {}

    Area(const Area& other)
        : uid(other.uid), gps(new GPS(*other.gps)),
        nehnutelnost(other.nehnutelnost ? new Nehnutelnost(*other.nehnutelnost) : nullptr),
        parcela(other.parcela ? new Parcela(*other.parcela) : nullptr) {}

    ~Area() {
        delete gps;
        delete nehnutelnost;
        delete parcela;
    }

    bool equals(const Area& other) const override {
        return this->uid == other.uid && this->gps->equalsByKeys(*other.gps);
    }

    bool equalsByKeys(const Area& other) const override {
        return this->gps->equalsByKeys(*other.gps);
    }

    int compare(const Area& other, int cur_level) const override {
        return gps->compare(*other.gps, cur_level);
    }

    friend std::ostream& operator<<(std::ostream& os, const Area& area) {
        os << "Area(uid: " << area.uid << ", GPS: " << *area.gps;
        if (area.nehnutelnost) os << ", Nehnutelnost: " << *area.nehnutelnost;
        if (area.parcela) os << ", Parcela: " << *area.parcela;
        os << ")";
        return os;
    }
};
