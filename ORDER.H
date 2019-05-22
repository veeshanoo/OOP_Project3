#pragma once
#include <bits/stdc++.h>

using namespace std;

class Order {
private:
    int id;
    double xPos;
    double yPos;
    double x;
    double y;
    double weight;
    bool isCold;
    double time; // day-time or delta(time) for 0 and 1 types
    double timer; // time when an order was received
    /// 0   ->   till some time
    /// 1   ->   in some time after picking up
    /// 2   ->   ASAP
    int type; // 0 1 2
    bool taken;
public:
    Order() : id(0), xPos(0), yPos(0), x(0), y(0), weight(0), isCold(0), time(0), timer(0), type(0), taken(0) {}
    
    Order(int i, double x, double y, double xx, double yy, double w, bool c, double tmp, double t, int tp) :
    id(i), xPos(x), yPos(y), x(xx), y(yy), weight(w), isCold(c), time(tmp), timer(t), type(tp), taken(0) {}

    Order(const Order& other) {
        id = other.id;
        xPos = other.xPos;
        yPos = other.yPos;
        x = other.x;
        y = other.y;
        weight = other.weight;
        isCold = other.isCold;
        time = other.time;
        timer = other.timer;
        type = other.type;
        taken = other.taken;
    }

    Order& operator = (const Order& other) {
        if (this != &other) {
            id = other.id;
            xPos = other.xPos;
            yPos = other.yPos;
            x = other.x;
            y = other.y;
            weight = other.weight;
            isCold = other.isCold;
            time = other.time;
            timer = other.timer;
            type = other.type;
            taken = other.taken;
        }
        return *this;
    }

    bool operator < (const Order& other) const {
        if (type == other.type)
            return timer > other.timer;
        if (type == 1 && other.type == 0)
            return 1;
        if (type == 1 && other.type == 2)
            return 1;
        if (type == 0 && other.type == 2)
            return 1;
        return 0;
    }

    friend istream& operator >> (istream&, Order&);

    friend class Vehicle;
    template <int maxT> friend class Manager;
};


istream& operator >> (istream& is, Order& obj) {
    is >> obj.id >> obj.timer >> obj.xPos >> obj.yPos >> obj.x >> obj.y >> obj.weight >> obj.isCold >> obj.time >> obj.type;
    obj.taken = 0;
    return is;
}