#include "cell.h"
#include <iostream>
int getAdjacentIndex(const int& i) {
    if (i < 3)
        return i + 3;
    else
        return i - 3;
}
char cell::getMarble()const {
    return marble;
}
void cell::setMarble(const char& m) {
    if (m == 'O' || m == '@' || m == '+') {
        marble = m;
    }
}

cell* cell::getAdjacent(const int& i)const {
    return adjacent.at(i);
}
void cell::setAdjacent(cell* a, const int& i) {
    if (adjacent.at(i) == nullptr) {
        adjacent.at(i) = a;
        a->setAdjacent(this, getAdjacentIndex(i));
    }
}
string cell::getLocation() const {
    return location;
}

bool cell::validateLocation(const string& l) {
    bool validPosition = false;
    if (l.size() == 2 && l[0] >= 'A' && l[0] <= 'I' && l[1] >= '1' && l[1] <= '9') {
        if (l[0] >= 'A' && l[0] <= 'E') {
            if ((5 + l[0] - 'A') > (l[1] - '1'))// A5, A4 ...
                validPosition = true;
        }
        else
            if ((l[0] - 'F') < (l[1] - '1'))// F2, F3 ... 
                validPosition = true;
    }
    return validPosition;
}
void cell::setLocation(const string& l) {
    if (validateLocation(l))
        location = l;
    else {
        throw "Invalid Location!";
    }
}

cell::cell() {
    for (int i = 0; i < 6; i++) {
        adjacent.push_back(nullptr);
        distanceToEdge.push_back(0);
    }
}

void cell::setDistanceToEdge(const int& i) {
    if (i > 5 || i < 0)//invalid direction
        return;
    if (distanceToEdge[i] == 0) {//distance has not yet been calculated (min distznce to edge = 1)
        cell* cp = this;
        cell* np = adjacent[i];//point to next cell in direction of i (this cell CLOSER to edge)
        cell* nq = adjacent[getAdjacentIndex(i)];//poont to neighbour in opposite direction (this cell FURTHER to edge)

        if (np != nullptr && np->getDistanceToEdge(i) != 0) {//if cp is not at edge of board and np's distance has been calculated
            distanceToEdge[i] = np->getDistanceToEdge(i) + 1;
        }
        else if (nq != nullptr && nq->getDistanceToEdge(i) != 0) {//if cell exists before current cell and its distance is known
            distanceToEdge[i] = nq->getDistanceToEdge(i) - 1;
        }
        else {//the next and previous cell's distnaces werent calcualted, loop through the cells in i direction and increment distanceToEdge
            while (cp != nullptr) {
                cp = cp->getAdjacent(i);
                distanceToEdge[i]++;
            }
        }
    }
}

int cell::getDistanceToEdge(const int& i) const {
    return  distanceToEdge.at(i);//at - does exception handeling if no value at i
}
