#pragma once
#include "VEHICLE.h"

class Van : public Vehicle {
public:
    Van() : Vehicle() {
        speed = 2;
    }

    Van(double x, double y, double w, int d, double t, bool f) : Vehicle(x, y, w, d, t) {
        speed = 2;
        frigo = f;
        movingType = 1;
    } 
};