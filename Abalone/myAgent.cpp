#include "myAgent.h"

#include <iostream>
using namespace std;

myAgent::myAgent(const char& symbol) : m(symbol) {}

movement myAgent::move(movement const* const om,const board& bd) {
	bool validMove = false;
	int mtype = 0;//default to in-line with no following marbles
	bool scoreMove = false;
	int d2e = 0;
	stringstream ss;
	string input = "a";
	
	while (!validMove) {
		if (input.length() != 0) {
			cout << "Enter Move: ";
		}
		
		getline(cin, input);

		
		if (input.find("q") == 0) {
			return movement("q", 0, 0, 0, 0, 0, false);//game should be exited
		}

		if (input.length() == 10) {
			m = input.substr(0, 1)[0];
			stringstream ss;
			ss << input[2] << input[3];
			l = ss.str();
			n = stoi(input.substr(5, 6));
			fd = stoi(input.substr(7, 8));
			md = stoi(input.substr(9, 10));
			validMove = bd.validateMove(m, l, n, fd, md, mtype, scoreMove);
			if (validMove) break;
		}

	}
	return movement(l, n, fd, md, d2e, mtype, scoreMove);
}
