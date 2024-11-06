#pragma once
#include <iostream>
using namespace std;

template<typename T>
class IComparable {
public:
    virtual int compare(const T& other, int cur_level) const = 0;
    virtual bool equals(const T& other) const = 0;
    virtual bool equalsByKeys(const T& other) const = 0;
    virtual ~IComparable() = default;
};

#include <iostream>
#include <ostream>

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
            return 0;
        } else if (cur_level % 4 == 1) {
            if (this->length < other.length) return -1;
            if (this->length > other.length) return 1;
            return 0;
        } else if (cur_level % 4 == 2) {
            if (this->x < other.x) return -1;
            if (this->x > other.x) return 1;
            return 0;
        } else if (cur_level % 4 == 3) {
            if (this->y < other.y) return -1;
            if (this->y > other.y) return 1;
            return 0;
        }
        return 0;
    }

    bool equalsByKeys(const GPS& other) const override {
        return this->x == other.x && this->y == other.y && this->width == other.width && this->length == other.length;
    }

    bool equals(const GPS& other) const override {
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
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Nehnutelnost& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    int compare(const Nehnutelnost& other, int cur_level) const override {
        if (cur_level % 4 == 0) {
            if (this->gps->width < other.gps->width) return -1;
            if (this->gps->width > other.gps->width) return 1;
        } else if (cur_level % 4 == 1) {
            if (this->gps->length < other.gps->length) return -1;
            if (this->gps->length > other.gps->length) return 1;
        } else if (cur_level % 4 == 2) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
        } else if (cur_level % 4 == 3) {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
        }
        return 0;
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
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Parcela& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    int compare(const Parcela& other, int cur_level) const override {
        if (cur_level % 4 == 0) {
            if (this->gps->width < other.gps->width) return -1;
            if (this->gps->width > other.gps->width) return 1;
        } else if (cur_level % 4 == 1) {
            if (this->gps->length < other.gps->length) return -1;
            if (this->gps->length > other.gps->length) return 1;
        } else if (cur_level % 4 == 2) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
        } else if (cur_level % 4 == 3) {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
        }
        return 0;
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

    int compare(const Area& other, int cur_level) const override {
        if (cur_level % 4 == 0) {
            if (this->gps->width < other.gps->width) return -1;
            if (this->gps->width > other.gps->width) return 1;
        } else if (cur_level % 4 == 1) {
            if (this->gps->length < other.gps->length) return -1;
            if (this->gps->length > other.gps->length) return 1;
        } else if (cur_level % 4 == 2) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
        } else if (cur_level % 4 == 3) {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
        }
        return 0;
    }

    bool equals(const Area& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Area& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Area& area) {
        os << "Area(uid: " << area.uid << ", GPS: " << *area.gps;
        if (area.nehnutelnost) os << ", Nehnutelnost: " << *area.nehnutelnost;
        if (area.parcela) os << ", Parcela: " << *area.parcela;
        os << ")";
        return os;
    }
};


class TestClass : public IComparable<TestClass> {
public:
    int uid;
    double A;
    string B;
    int C;
    double D;

    TestClass(int id, double a, const string& b, int c, double d)
        : uid(id), A(a), B(b), C(c), D(d) {}

    TestClass(const TestClass& other) : uid(other.uid), A(other.A), B(other.B), C(other.C), D(other.D) {}

    int compare(const TestClass& other, int cur_level) const override {
        int cmpB = 0;
        switch (cur_level % 4) {
        case 0:
            if (A < other.A) return -1;
            if (A > other.A) return 1;
            return B.compare(other.B);
        case 1:
            if (C < other.C) return -1;
            if (C > other.C) return 1;
            return 0;
        case 2:
            if (D < other.D) return -1;
            if (D > other.D) return 1;
            return 0;
        case 3:
            cmpB = B.compare(other.B);
            if (cmpB != 0) return cmpB;
            if (C < other.C) return -1;
            if (C > other.C) return 1;
            return 0;
        default:
            return 0;
        }
    }

    bool equals(const TestClass& other) const override {
        return A == other.A && B == other.B && C == other.C && D == other.D && uid == other.uid;
    }

    bool equalsByKeys(const TestClass& other) const override {
        return A == other.A && B == other.B && C == other.C && D == other.D;
    }

    friend ostream& operator<<(ostream& os, const TestClass& testClass) {
        os << "TestClass(uid: " << testClass.uid << ", A: " << testClass.A
           << ", B: " << testClass.B << ", C: " << testClass.C
           << ", D: " << testClass.D << ")";
        return os;
    }
};
