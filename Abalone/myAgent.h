#pragma once

#ifndef MY_AGENT_H
#define MY_AGENT_H
#include <cstdlib>
#include <string>
#include "Board.h"
#include "Agent.h"
using namespace std;

class myAgent {
	char m;//marble character
	int n;
	int fd;
	int md;
	string l;
public:
	myAgent(const char& symbol);
	void requestMove(const int& moveNum);
	char getMarble() const{return m;} 
	movement move(movement const* const om, const board& bd);
};


#endif