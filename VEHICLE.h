#pragma once 
#include <bits/stdc++.h>
#include "ORDER.h"

using namespace std;

// v = d / t    =>   t = d / v

class Vehicle {
protected:
    double xPos;
    double yPos;
    double maxWeight;
    double weight;
    double speed;
    int movingType; // 0 euclidean 1 manhattan
    /// 0   ->   ASAP
    /// 1   ->   closest order 
    /// 2   ->   first come first served 
    int deliveryType; // 0 1 2 
    bool frigo;
    double timer;
    vector<Order> v; 
public:
    Vehicle() : xPos(0), yPos(0), maxWeight(0), weight(0), movingType(0), deliveryType(0), frigo(0), timer(0) {}

    Vehicle(double x, double y, double w, int d, double t) 
    : xPos(x), yPos(y), maxWeight(w), weight(0), deliveryType(d), timer(t) {}

    Vehicle(const Vehicle& other) {
        xPos = other.xPos;
        yPos = other.yPos;
        maxWeight = other.maxWeight;
        weight = other.weight;
        speed = other.speed;
        movingType = other.movingType;
        deliveryType = other.deliveryType;
        frigo = other.frigo;
        timer = other.timer;
        for (auto it : other.v)
            v.push_back(it);
    }

    Vehicle& operator = (const Vehicle& other) {
        if (this != &other) {
            v.clear();
            xPos = other.xPos;
            yPos = other.yPos;
            maxWeight = other.maxWeight;
            weight = other.weight;
            speed = other.speed;
            movingType = other.movingType;
            deliveryType = other.deliveryType;
            frigo = other.frigo;
            timer = other.timer;
            for (auto it : other.v)
                v.push_back(it);
        }
        return *this;
    }

    double manhattanDistance(double, double);
    double manhattanDistance(double, double, double, double);
    double euclideanDistance(double, double);
    double euclideanDistance(double, double, double, double);
    double getDistance(double, double);
    double getDistance(double, double, double, double);
    double getTime(double, double);
    double getTime(double, double, double, double);

    void setDeliveryType(int);
    void pickClosest();

    double getDeliveryTime(Order, const double&);
    void addOrder(Order&);

    void updateTimer(double);

    virtual ~Vehicle() {}

    template <int maxT> friend class Manager;
};

/// Distance and time functions 
double Vehicle::manhattanDistance(double x, double y) {
    double ans = abs(xPos - x) + abs(yPos - y);
    return ans;
}

double Vehicle::manhattanDistance(double x, double y, double xx, double yy) {
    double ans = abs(xx - x) + abs(yy - y);
    return ans;
}

double Vehicle::euclideanDistance(double x, double y) {
    double ans = (xPos - x) * (xPos - x) + (yPos - y) * (yPos - y);
    return sqrt(ans);
}

double Vehicle::euclideanDistance(double x, double y, double xx, double yy) {
    double ans = (xx - x) * (xx - x) + (yy - y) * (yy - y);
    return sqrt(ans);
}

double Vehicle::getDistance(double x, double y) {
    if (movingType == 0)
        return euclideanDistance(x, y);
    return manhattanDistance(x, y);
}

double Vehicle::getDistance(double x, double y, double xx, double yy) {
    if (movingType == 0)
        return euclideanDistance(x, y, xx, yy);
    return manhattanDistance(x, y, xx, yy);
}

double Vehicle::getTime(double x, double y) {
    double d = getDistance(x, y);
    return d / speed;
}

double Vehicle::getTime(double x, double y, double xx, double yy) {
    double d = getDistance(x, y, xx, yy);
    return d / speed;
}

/// Changes vehicle delivery type
void Vehicle::setDeliveryType(int x) {
    if (v.size() > 0)
        return;
    deliveryType = x;
}

/// v.back() becomest the order closest to the vehicle
void Vehicle::pickClosest() {
    if (v.size() == 0)
        return;
    double mn = INT_MAX, id = -1;
    for (int i = 0; i < v.size(); i++) {
        double dist;
        if (v[i].taken == 1)
            dist = getDistance(v[i].x, v[i].y);
        else dist = getDistance(v[i].xPos, v[i].yPos);
        if (dist < mn) {
            mn = dist;
            id = i;
        }
    }
    while (id < v.size() - 1) {
        swap(v[id], v[id + 1]);
        id++;
    }
}

/// Finds minimum time to deliver an order and checks if an order can be assigned to a car 
/// Returns maxT if it's impossible to assign the order
double Vehicle::getDeliveryTime(Order obj, const double& maxT) {
    if (weight + obj.weight > maxWeight)
        return maxT + 1;
    if (obj.isCold && !frigo)
        return maxT + 1;
    Vehicle aux = *this;
    double tmp = aux.timer;
    switch (deliveryType) {
        
        /// 0   ->   ASAP
        case 0 :
            /// mark obj as special 
            obj.weight = -1;
            aux.v.push_back(obj);
            sort(aux.v.begin(), aux.v.end());
            while (aux.v.size() > 0) {
                Order o = aux.v.back(); 

                if (o.taken == 0) {
                    aux.timer += getTime(o.xPos, o.yPos);
                    aux.timer = ceil(aux.timer);
                    aux.xPos = o.xPos;
                    aux.yPos = o.yPos;
                    aux.v.back().taken = 1;
                    if (o.type == 1)
                        aux.v.back().time += aux.timer;
                } else {
                    aux.timer += getTime(o.x, o.y);
                    aux.timer = ceil(aux.timer);
                    aux.xPos = o.x;
                    aux.yPos = o.y;        
                    aux.v.pop_back();

                    if (o.weight == -1)
                        tmp = aux.timer;

                    if (o.type == 0 && aux.timer > o.time)
                        return maxT + 1;
                    if (o.type == 1 && aux.timer > o.time)
                        return maxT + 1;
                    if (aux.timer > maxT)
                        return maxT + 1;
                }    
            }

            return tmp;
        
        /// 1   ->   closest order 
        case 1 :
            /// mark obj as special
            obj.weight = -1;
            aux.v.push_back(obj);
            while (aux.v.size() > 0) {
                aux.pickClosest();
                Order o = aux.v.back(); 

                if (o.taken == 0) {
                    aux.timer += getTime(o.xPos, o.yPos);
                    aux.timer = ceil(aux.timer);
                    aux.xPos = o.xPos;
                    aux.yPos = o.yPos;
                    aux.v.back().taken = 1;
                    
                    if (o.type == 1)
                        aux.v.back().time = o.time += aux.timer;

                } else {
                    aux.timer += getTime(o.x, o.y);
                    aux.timer = ceil(aux.timer);
                    aux.xPos = o.x;
                    aux.yPos = o.y;        
                    aux.v.pop_back();

                    if (aux.weight == -1)
                        tmp = aux.timer;

                    if (o.type == 0 && aux.timer > o.time)
                        return maxT + 1;
                    if (o.type == 1 && aux.timer > o.time)
                        return maxT + 1;
                    if (aux.timer > maxT)
                        return maxT + 1;
                }    
            }

            return tmp;
         
        /// 2   ->   first come first served 
        case 2 :
            while (aux.v.size() > 0) {
                Order o = aux.v.back();
                aux.v.pop_back();
                
                if (o.taken == 0) { 
                    aux.timer += getTime(o.xPos, o.yPos);
                    aux.timer = ceil(aux.timer);
                    aux.xPos = o.xPos;
                    aux.yPos = o.yPos;
                    o.taken = 1;
                } 
                
                aux.timer += getTime(o.x, o.y);
                aux.timer = ceil(aux.timer);
                aux.xPos = o.x;
                aux.yPos = o.y;
            }

            aux.timer += getTime(obj.xPos, obj.yPos);
            aux.timer = ceil(aux.timer);
            tmp = aux.timer;
            aux.xPos = obj.xPos;
            aux.yPos = obj.yPos;
            aux.timer += getTime(obj.x, obj.y);
            aux.timer = ceil(aux.timer);
            tmp = aux.timer - tmp;
            
            if (obj.type == 0 && aux.timer > obj.time)
                return maxT + 1;
            if (obj.type == 1 && tmp > obj.time)
                return maxT + 1;
            if (aux.timer > maxT)
                return maxT + 1;
            
            return aux.timer;
    }
}

/// Assigns an order 
void Vehicle::addOrder(Order& obj) {
    weight += obj.weight;
    v.push_back(obj);
    switch (deliveryType) {
        case 0 :
            sort(v.begin(), v.end());
            break;
        case 1 :
            pickClosest();
            break;
        case 2 :
            for (int i = v.size() - 1; i > 0; i--)
                swap(v[i], v[i - 1]);
            break;
    }
}

/// Delivers or picks orders
void Vehicle::updateTimer(double t) {
    if (v.size() == 0) {
        timer = t;
        return;
    }
    switch (deliveryType) {
        case 0 :
            sort(v.begin(), v.end());
            while (v.size() > 0) {
                Order o = v.back();
                double tmp = timer;
                if (o.taken == 0) {
                    tmp += getTime(o.xPos, o.yPos);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.back().taken = 1;
                        if (o.type == 1) 
                            v.back().time += tmp;
                        cout << "Order with id " << o.id << ' ' << " was picked at " << tmp << '\n';
                    } else 
                        return;
                } else {
                    tmp += getTime(o.x, o.y);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.pop_back();
                        weight -= o.weight;
                        cout << "Order with id " << o.id << ' ' << " was delivered at " << tmp << '\n';
                    } else 
                        return;
                } 
            }
            return;
        case 1 :
            while (v.size() > 0) {
                pickClosest();
                Order o = v.back();
                double tmp = timer;
                if (o.taken == 0) {
                    tmp += getTime(o.xPos, o.yPos);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.back().taken = 1;
                        if (o.type == 1) 
                            v.back().time += tmp;
                        cout << "Order with id " << o.id << ' ' << " was picked at " << tmp << '\n';
                    } else 
                        return;
                } else {
                    tmp += getTime(o.x, o.y);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.pop_back();
                        weight -= o.weight;
                        cout << "Order with id " << o.id << ' ' << " was delivered at " << tmp << '\n';
                    } else 
                        return;
                } 
            }
            return;
        case 2 :
            while (v.size() > 0) {
                Order o = v.back();
                double tmp = timer;
                if (o.taken == 0) {
                    tmp += getTime(o.xPos, o.yPos);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.back().taken = 1;
                        if (o.type == 1) 
                            v.back().time += tmp;
                        cout << "Order with id " << o.id << ' ' << " was picked at " << tmp << '\n';
                    } else 
                        return;
                } else {
                    tmp += getTime(o.x, o.y);
                    tmp = ceil(tmp);
                    if (tmp <= t) {
                        timer = tmp;
                        v.pop_back();
                        weight -= o.weight;
                        cout << "Order with id " << o.id << ' ' << " was delivered at " << tmp << '\n';
                    } else 
                        return;
                } 
            }
            return;
    }
}