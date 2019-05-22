#pragma once
#include <bits/stdc++.h>
#include "VEHICLE.h"
#include "CAR.h"
#include "VAN.h"
#include "SCOOTER.h"

using namespace std;

template <int maxT = 86400>
class Manager {
private:
    /// 0   ->   closest vehicle
    /// 1   ->   fastest delivery
    /// 2   ->   least loaded vehicle
    int strategy; // 0 1 2
    static Manager* ptr;
    Manager() : strategy(0) {}
    Manager(int s) : strategy(s) {}
public:

    static Manager* getInstance() {
        if (ptr == NULL)
            ptr = new Manager;
        return ptr;
    }

    void changeStrategy(int s) {
        strategy = s;
    }

    void Work(istream&);

    ~Manager() {}
};

template <int maxT>
void Manager<maxT>::Work(istream& is) {
    Vehicle* c[3];
    c[0] = new Car(0, 0, 250, 0, 0);
    c[1] = new Van(50, 50, 1000, 0, 0, 1);
    c[2] = new Scooter(50, 50, 60, 0, 0);
    int n;
    is >> n;
    for (int test = 1; test <= n + 1; test++) {
        if (test == n + 1) {
            for (int i = 0; i < 3; i++)
                c[i]->updateTimer(maxT);
            return;
        }
        Order o;
        is >> o;
        if (o.type == 2)
            changeStrategy(1);
        
        for (int i = 0; i < 3; i++)
            c[i]->updateTimer(o.timer);

        switch (strategy) {
            case 0 : 
                {
                    double bst = DBL_MAX;
                    int id = -1;
                    for (int i = 0; i < 3; i++) {
                        double dist = c[i]->getDistance(o.xPos, o.yPos);
                        double tmp = c[i]->getDeliveryTime(o, maxT);
                        if (dist < bst && tmp <= maxT) {
                            bst = dist;
                            id = i;
                        }
                    }
                    if (id != -1)
                        c[id]->addOrder(o);
                    else cout << "Order with id " << o.id << " couldn't be delivered, sorry.\n";
                }  
                break;
            case 1 :
                {
                    double bst = maxT + 1;
                    int id = -1;
                    for (int i = 0; i < 3; i++) {
                        double tmp = c[i]->getDeliveryTime(o, maxT);
                        if (tmp <= maxT) {
                            bst = tmp;
                            id = i;
                        }
                    }
                    if (id != -1) 
                        c[id]->addOrder(o);
                    else cout << "Order with id " << o.id << " couldn't be delivered, sorry.\n";
                }
                break;
            case 2 : 
                {
                    double bst = 0;
                    int id = -1;
                    for (int i = 0; i < 3; i++) {
                        double w = c[i]->maxWeight - c[i]->weight;
                        double tmp = c[i]->getDeliveryTime(o, maxT);
                        if (w > bst && tmp <= maxT) {
                            bst = w;
                            id = i;
                        }
                    }
                    if (id != -1)
                        c[id]->addOrder(o);
                    else cout << "Order with id " << o.id << " couldn't be delivered, sorry.\n";
                }
                break;
        }
    }
}