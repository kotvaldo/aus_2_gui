#include "Parameters.h"
#include "Util.h"

void GPSParameters::randomize() {
    x = Util::randomNumber(0.0, 90.0);
    y = Util::randomNumber(0.0, 180.0);
    width = (Util::randomNumber(0, 1) == 0) ? 'N' : 'S';
    length = (Util::randomNumber(0, 1) == 0) ? 'E' : 'W';
}

void ObjParameters::randomize() {
    number = Util::randomNumber(1, 1000);
    description = Util::randomString(10);
}
