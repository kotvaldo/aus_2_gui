#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

struct GPSParameters
{
    double x;
    double y;
    char width;
    char length;
};


struct ObjParameters
{
    int number;
    std::string description;
};


#endif // PARAMETERS_H
