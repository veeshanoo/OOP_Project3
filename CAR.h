#pragma once 
#include "VEHICLE.h"

class Car : public Vehicle {
public:
    Car() : Vehicle() {
        speed = 4;
    }

    Car(double x, double y, double w, int d, double t) : Vehicle(x, y, w, d, t) {
        speed = 4;
        frigo = 0;
        movingType = 1;
    } 
};

