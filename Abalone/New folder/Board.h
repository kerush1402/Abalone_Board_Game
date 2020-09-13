#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include <map>
#include <sstream>

using namespace std;

class board {
    vector<cell*> rows;
    map<string, cell*> cells;
    int woc;//white marbles off the board count.
    int boc;//black marbles off the board count.
    char playerTurn = 'O';//first move should be the white marble
public:
    board();//create 61 cells on the heap, and connect them.
    board(const board&);//copy constructor
    ~board();//recycle 61 cells.
    map<string, cell*> getCells() const;
    operator std::string() const;//cast this object into a string.
    string traverseHorizontal() const;
    string traverseDiagonal() const;
    bool validateMove(const char&, const string& l, const int& n, const int& fd, const int& md, int& mtype, bool& scoreMove) const;
    bool executeMove(const char&, const string& l, const int& n, const int& fd, const int& md);
    int getWoc() {return woc;}
    int getBoc() {return boc;}
    char getTurn() const { return playerTurn; }
    void setTurn(char m) { playerTurn = m; }
    bool inPlay();
};



#endif /* BOARD_H */

