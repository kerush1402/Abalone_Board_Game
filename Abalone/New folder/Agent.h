#pragma once

#ifndef AGENT_H
#define AGENT_H

#include "board.h"
#include "cell.h"
#include <set>
#include <algorithm>
#include <cmath>
#include <cstdlib>

struct movement {
public:
    string l;//location
    int n;//number of current player's marbles moving
    int fd;//direction from leading marbles to following marbles
    int md;//direction in which n marbles seek to move
    int d2e;//distance from leading marble to the edge of the board in move direction
    int mtype;//-1 broad-side 0-2 for in-line depending on how many opponent marbles are to be pushed
    bool scoreMove;//opponent marble to be pushed off the board?
    movement(const string&, const int&, const int&, const int&, const int&, const int&, const bool&);
    movement(const movement&);
    movement();
};


struct moveTNDPriority {//priority based on move types and distance to edge
    double score(const movement&) const;
    bool operator()(const movement& a, const movement& b) const;
};


class agent {
protected:
    char symbol;
public:
    vector<vector<int> > pmd;//store every possible move that can be made

    vector<movement> findMoves(const board&) const;//find avail moves that can be made at current board state
public:
    agent(const char&);
    char getSymbol() { return symbol; }
    virtual movement move(movement const* const, board) = 0;//pure virtual function
};


class randAgent : public agent {
public:
    randAgent(const char&);
    movement move(movement const* const, board);
};


class distAgent : public agent {
public:
    distAgent(const char&);
    movement move(movement const* const, board);
};


#endif