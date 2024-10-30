#pragma once

#include <iostream>
#include "GPS.h"
using namespace std;


class Parcela {
public:
    int cisloParcely;
    string popis;
    GPS* pozicia;
    GPS* pozicia2;

    Parcela(int cisloParcely, const string& popis, GPS* pozicia, GPS* pozicia2)
        : cisloParcely(cisloParcely), popis(popis), pozicia(pozicia), pozicia2(pozicia2)
    {
    }

};

class Nehnutelnost {
public:
    int supisneCislo;
    string popis;
    GPS* pozicia;
    GPS* pozicia2;

   


    Nehnutelnost(int supisneCislo, const string& popis, GPS* pozicia, GPS* pozicia2)
        : supisneCislo(supisneCislo), popis(popis), pozicia(pozicia), pozicia2(pozicia2)
    {
    }
};
