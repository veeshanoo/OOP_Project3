#include <bits/stdc++.h>
#include "MANAGER.h"

using namespace std;

const int maxT = 86400;

template <> Manager<maxT>* Manager<maxT>::ptr = NULL;

int main() {
    ifstream cin("tst.in");
    Manager<maxT>::getInstance()->Work(cin);
    return 0;
}