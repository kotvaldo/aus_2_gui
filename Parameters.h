#pragma once

#include <string>
using namespace std;

//CRATE DP


class Parameters {
public:
    virtual ~Parameters() = default;
    virtual void randomize();
};

struct GPSParameters : public Parameters {
    double x;
    double y;
    char width;
    char length;

    GPSParameters(double x = 0, double y = 0, char width = 'N', char length = 'W')
        : x(x), y(y), width(width), length(length) {}

   void randomize() override;

};


struct ObjParameters : public Parameters {
    int id = -1;
    int number;
    std::string description;

    ObjParameters(int number = 0, const std::string& description = "")
        : number(number), description(description) {}

    void randomize() override;
};


