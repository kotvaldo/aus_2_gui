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

class GPS : public IComparable<GPS> {
public:
    int x, y;

    GPS(int x = 0, int y = 0) : x(x), y(y) {}

    GPS(const GPS& other) : x(other.x), y(other.y) {}

    int compare(const GPS& other, int cur_level) const override {
        if (cur_level % 2 == 0) {
            if (this->x < other.x) return -1;
            if (this->x > other.x) return 1;
            return 0;
        }
        else {
            if (this->y < other.y) return -1;
            if (this->y > other.y) return 1;
            return 0;
        }
    }

    bool equalsByKeys(const GPS& other) const override {
        return this->x == other.x && this->y == other.y;
    }

    bool equals(const GPS& other) const override {
        return this->x == other.x && this->y == other.y;
    }

    friend ostream& operator<<(ostream& os, const GPS& gps) {
        os << "(" << gps.x << ", " << gps.y << ")";
        return os;
    }
};

class Nehnutelnost : public IComparable<Nehnutelnost> {
public:
    int uid;
    GPS* gps;
    string nazov; 

    Nehnutelnost(int id, GPS* gpsCoord, const string& name = "")
        : uid(id), gps(new GPS(*gpsCoord)), nazov(name) {}

   
    Nehnutelnost(const Nehnutelnost& other)
        : uid(other.uid), gps(new GPS(*other.gps)), nazov(other.nazov) {}

    ~Nehnutelnost() { delete gps; }

    bool equals(const Nehnutelnost& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Nehnutelnost& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    int compare(const Nehnutelnost& other, int cur_level) const override {
        if (cur_level % 2 == 0) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }

    // Výpis objektu Nehnutelnost vrátane názvu
    friend ostream& operator<<(ostream& os, const Nehnutelnost& nehnutelnost) {
        os << "Nehnutelnost(uid: " << nehnutelnost.uid
            << ", GPS: " << *nehnutelnost.gps
            << ", Name: " << (nehnutelnost.nazov.empty() ? "Unnamed" : nehnutelnost.nazov) << ")";
        return os;
    }
};


class Parcela : public IComparable<Parcela> {
public:
    int uid;
    GPS* gps;

    Parcela(int id, GPS* gpsCoord) : uid(id), gps(new GPS(*gpsCoord)) {}

    // Copy constructor for deep copy
    Parcela(const Parcela& other) : uid(other.uid), gps(new GPS(*other.gps)) {}

    ~Parcela() { delete gps; }

    bool equals(const Parcela& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Parcela& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    int compare(const Parcela& other, int cur_level) const override {
        if (cur_level % 2 == 0) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }

    friend ostream& operator<<(ostream& os, const Parcela& parcela) {
        os << "Parcela(uid: " << parcela.uid << ", GPS: " << *parcela.gps << ")";
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
        : uid(id), gps(new GPS(*gpsCoord)), nehnutelnost(nehnut ? new Nehnutelnost(*nehnut) : nullptr), parcela(parc ? new Parcela(*parc) : nullptr) {}

    // Copy constructor for deep copy
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
        if (cur_level % 2 == 0) {
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }

    bool equals(const Area& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    bool equalsByKeys(const Area& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y;
    }

    friend ostream& operator<<(ostream& os, const Area& area) {
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

    // Copy constructor for deep copy
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
