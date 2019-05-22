#pragma once
#include "VEHICLE.h"

class Scooter : public Vehicle {
public:
    Scooter() : Vehicle() {
        speed = 8;
    }

    Scooter(double x, double y, double w, int d, double t) : Vehicle(x, y, w, d, t) {
        speed = 3;
        frigo = 0;
        movingType = 0;
    } 
};