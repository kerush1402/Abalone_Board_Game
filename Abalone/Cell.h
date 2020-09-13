#pragma once
#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>

using namespace std;
int getAdjacentIndex(const int& i);
class cell {
    char marble;
    vector<cell*> adjacent;//six directions: north-east clockwise to north-west
    string location;
    vector<int> distanceToEdge; 
public:
    char getMarble() const;
    void setMarble(const char&);//set marble to either O, @ or +.
    cell* getAdjacent(const int&) const;
    void setAdjacent(cell*, const int&);//only set each neighbour once.
    string getLocation() const;
    static bool validateLocation(const string& l);
    void setLocation(const string& l);
    void setDistanceToEdge(const int& i);
    int getDistanceToEdge(const int& i) const;
    cell();//set all 6 neighbours to null, and all distance to edge set to 0 
};

#endif /* CELL_H */

