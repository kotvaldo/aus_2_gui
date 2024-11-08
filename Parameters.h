#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

struct GPSParameters {
    double x;
    double y;
    char width;
    char length;

};

struct BoundingBox {
    GPSParameters point1;
    GPSParameters point2;

    BoundingBox(GPSParameters point1, GPSParameters point2)
        : point1(point1), point2(point2) {}
};


struct ParcelaParameters {
    int cisloParcely;
    std::string description;
};

struct NehnutelnostParameters {
    int supisneCislo;
    std::string description;

};



#endif // PARAMETERS_H
