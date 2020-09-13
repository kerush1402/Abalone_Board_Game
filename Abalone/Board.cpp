#include "board.h"
#include <iomanip>
#include <iostream>

void connectHorizontal(cell* row, const int& rowSize)
{
	// -1 becuase no cell to right of last cell
	for (int i = 0; i < rowSize - 1; i++)
	{
		/*connect current cell to next cell, and in turn it that cell will connect to this cell*/
		row[i].setAdjacent(row + i + 1, 1);//next cell at i'th address and direction is East
	}
}


/*f = either first cell or second cell of row, Call function with f=0 and f=1 to connect NE AND NW for both top and bottom*/
void connectDiagonal(cell* lrow, cell* srow, const int& f, const int& srowSize, const bool& top)
{
	for (int i = f; i < (f + srowSize); i++)/*r + srowSize = all elements in shorter row*/
	{
		if (top)/*if in top half,longer row is in bottom*/
		{
			if (f == 0)/*starting at first cell in longer row,can only connect NE, no NW */
				lrow[i].setAdjacent(srow + i, 0);
			else if (f == 1)/*starting from second cell, so connect to NW*/
				lrow[i].setAdjacent(srow + i - 1, 5);/*-1 because NW direction points to previous position of i in shorter row*/
			else return;
		}
		else/*top row is longer row*/
		{
			if (f == 0)/*first cell, only can set SE, SW is invalid*/
				lrow[i].setAdjacent(srow + i, 2);
			else if (f == 1)
				lrow[i].setAdjacent(srow + i - 1, 3);/*set SE direction if second cell and connect to cell i - 1 in short row*/
			else return;
		}
	}
}


/*add the cell with its location to map when board is created*/
void mapCells(cell* row, map<string, cell*>& cm, const char& r, const int& rowSize)
{
	int f = 1;//left most diagonal

	if (r >= 'A' && r <= 'E')/*bottom half, all rows start with diagonal 1*/
	{
		f = 1;
	}
	else if (r > 'E' && r <= 'I')/*top half, f starts with 2 and increments with each row change*/
	{
		f = r - 'E' + 1;/*diagnonal increment realative to distance from E, eg: F - E + 1 = 2*/
	}
	else return;

	/*loop through cells in row and record row name and current diagonal*/
	for (int i = 0; i < rowSize; i++)
	{
		stringstream ss;
		ss << r; //row name/letter
		ss << i + f; //current diagonal
		string lc = ss.str();//copy ss to a string
		row[i].setLocation(lc); //set that cell location
		cm[lc] = row + i;//add cell address to map with key=location
	}
}

map<string, cell*> board::getCells() const {
	return cells;
}

void setMarbles(cell* row, const int& rowSize, const char m)
{
	for (int i = 0; i < rowSize; i++)
		row[i].setMarble(m); //for each cell cell::setMarble
	//cout << rowSize <<endl;
	if (rowSize == 7)/*4 cells are empty if rowsize = 7*/
	{
		char posEmptyCells[4] = { 0,1,5,6 };
		for (int i = 0; i < 4; i++)
			row[posEmptyCells[i]].setMarble('+');
	}
}


board::~board() {
	for (int i = 0; i < 9; i++) {
		delete[] rows[i];
		rows[i] = nullptr;
	}
	cells.clear();
}


board::board() :woc(0), boc(0)
{
	char r = 'I';

	for (int i = 0; i < 9; i++)
	{
		/*Top half A-E*/
		if (i < 5)
		{
			/*1.allocate space for cells per row*/
			rows.push_back(new cell[5 + i]);
			/*2. connect them horizintal,pass pointer to first cell of row and row size, Cant connect diagonal coz bottom row not created yet*/
			connectHorizontal(rows[i], 5 + i);
			/*3.add it top map using method mapCells,pass pointer to first element of i'th row, map by ref, row letter and row size*/
			mapCells(rows[i], cells, r--, 5 + i);
			/*4.set the marbles for the i'th row, either WHite(O)(first 3 rows) or empty(+)*/
			if (i < 3)
				setMarbles(rows[i], 5 + i, 'O');
			else
				setMarbles(rows[i], 5 + i, '+');
		}
		else/*bototm half F-I*/
		{
			rows.push_back(new cell[5 + 8 - i]);
			connectHorizontal(rows[i], 5 + 8 - i);
			mapCells(rows[i], cells, r--, 5 + 8 - i);
			if (i >= 6)
				setMarbles(rows[i], 5 + 8 - i, '@');
			else
				setMarbles(rows[i], 5 + 8 - i, '+');
		}

	}

	/*set the diagonal connections,start at one, connect upwards and rows[0] has no NE*/
	for (int i = 1; i < 9; i++)
	{
		if (i < 5)/*top half A-E*/
		{
			/*longer row = rows[i], shorter row(below) = rows[i-1] */
			connectDiagonal(rows[i], rows[i - 1], 0, 5 + i - 1, true);//0 = first cell, connect NE and SW
			connectDiagonal(rows[i], rows[i - 1], 1, 5 + i - 1, true);//1 = second cell, connect NW and SE
		}
		else/*bottom half F-I*/
		{
			connectDiagonal(rows[i - 1], rows[i], 0, 5 + 8 - i, false);//0 = first cell, connect NE and SW, shorter row is at bottom rows[i-1]
			connectDiagonal(rows[i - 1], rows[i], 1, 5 + 8 - i, false);//1 = second cell, connect NW and SE
		}
	}

	//set the distance to edge for each cell
	cell* linep = rows[0];//point to I5
	while (linep != nullptr) {
		//loop through the row
		cell* cellp = linep;
		while (cellp != nullptr) {
			//set all value of distanceToEdge in each of the directions 
			for (int i = 0; i < 6; i++) {
				cellp->setDistanceToEdge(i);
			}
			cellp = cellp->getAdjacent(1);//move to next cell in East direction
		}
		//move to next row
		if (linep->getAdjacent(3) == nullptr)
			linep = linep->getAdjacent(2);//reached middle of board
		else
			linep = linep->getAdjacent(3);
	}
	

	/*map<string, cell*>::const_iterator it = cells.find(string("C1"));
	cell* leadCell = it->second;
	leadCell->setMarble('O');
	leadCell = leadCell->getAdjacent(1);
	leadCell->setMarble('O');*/

}


board::board(const board& copy) : board() {//constructor delegation - C++11
	woc = copy.woc;
	boc = copy.boc;
	map<string, cell*>::const_iterator it = copy.cells.begin();
	while (it != copy.cells.end()) {
		(cells[it->first])->setMarble((*(it->second)).getMarble());
		it++;
	}
}


board::operator std::string() const {
	stringstream ss;
	char r = 'I';
	int d = 9;
	for (int i = 0; i < 9; i++) {
		if (i < 5) {
			ss << setw(5 - i) << r << " ";
			for (int k = 0; k < 5 + i; k++) {
				ss << rows[i][k].getMarble() << " ";
			}
			ss << endl;
			r--;
		}
		else {
			ss << setw(i - 3) << r << " ";
			for (int k = 0; k < 5 + 8 - i; k++) {
				ss << rows[i][k].getMarble() << " ";
			}
			ss << d << endl;
			r--;
			d--;
		}
	}
	d = 1;
	ss << setw(8) << d++ << " ";
	for (; d < 6; d++)
		ss << d << " ";
	ss << endl;
	for (int i = 0; i < woc; i++)
		ss << 'O' << " ";
	ss << endl;
	for (int i = 0; i < boc; i++)
		ss << '@' << " ";
	ss << endl;
	return ss.str();
}


string traverse(const cell* start, const int& pd, const int& sd, const int& td)
{/* Horizontal:     Diagonal:
	pd=5 NW         pd=0 NE
	sd=0 NE         sd=1 E      sd & pd out of bounds restrictions for traversal methods
	td=1 E          td=2 SE     td= direction want to traverse*/
	string ts;//string of traversal list
	const cell* linep = start;//linep = start

	/*while cell "start/linep" has a cell adjacent in Primary Dir , move that way else move Secondary Dir
	while (linep->getAdjacent(pd) != nullptr || linep->getAdjacent(sd) != nullptr) , doesnt traverse last row so
	do it after loop, OR method 2: (OPTIMIZED)*/
	while (linep != nullptr)
	{
		const cell* cellp = linep; //cellp = start

		/*if cell not empty, added it to string and move to next cell*/
		while (cellp != nullptr)
		{
			ts += (cellp->getLocation() + ' ');
			
			//disply distance to edge of board for each cell
			cout << cellp->getLocation() << "'s distance to edge ";
			for (int i = 0; i < 6; i++) {
				int dis = cellp->getDistanceToEdge(i);
				cout << dis << " ";
			}
			cout << endl;

			cellp = cellp->getAdjacent(td);//Horiz- move east
		}

		/*move to next line. *linep = start = first cell od row*/
		if (linep->getAdjacent(pd) == nullptr)/*if cell east NW is empty else move NE (Top half)-for horizn, for dia- if NE is empty,move east (reached the top row, no more NE*/
			linep = linep->getAdjacent(sd);
		else/*Else if there is NW, Move NW (Bottom half)-for horiz, for vertincal, if theres is NE, move NE*/
			linep = linep->getAdjacent(pd);
	}
	/*last row(top row) has not been traversed for horizontal. For vertical, reach top right (I9) and need to traverse diagonally for that cell
	const cell* cellp = linep;
	while (cellp != nullptr)
	{
		ts += (cellp->getLocation() + ' ');
		cellp = cellp->getAdjacent(td);
	}*/
	return ts;
}

string board::traverseHorizontal() const
{
	map<string, cell*>::const_iterator it = cells.find(string("A1"));
	cell* start = it->second; //(*it).second = cell address of E1
	return traverse(start, 5, 0, 1);
}

string board::traverseDiagonal() const
{
	map<string, cell*>::const_iterator it = cells.find(string("E1"));
	cell* start = it->second;
	return traverse(start, 0, 1, 2);
}


bool inlineMove(const int& numMarbles, const int& followDir, const int& moveDir) {
	/*
	followDir - direction of cells from lead marble, that want to move
	moveDir - direction in which to move the marbles
	inline move(the follow direction = move direction || follow dir opposite to move direction)
	*/
	if (followDir == moveDir || numMarbles == 1)
		return true;
	else {
		//check if they opposit each other
		int check = followDir;//check=0 so 0+3 = 3, now test if 3 == moveDir, if so, its inline move

		if (check < 3)
			check += 3;
		else
			check -= 3;

		if (check == moveDir)
			return true;
		else
			return false;
	}
}

bool board::validateMove(const char& m, const string& l, const int& n, const int& fd, const int& md, int& mtype, bool& scoreMove) const {
	/*first Paramter (player turn)*/
	/*if (m != this->getTurn()) {
		cout << "Not yout turn! " <<  this->getTurn()  << "'s turn"<< endl;
		return false;
	}*/

	/*second parameter (location)*/
	if (!cell::validateLocation(l)) {//confirm existence of location
		//cout << "Invalid location. No Marble at " << l << endl;
		return false;
	}
	cell* cp = cells.at(l);//initialize to leading cell
	const char marble = cp->getMarble();
	if (marble == '+') {//trying to move a space
		//cout << "No marble at location " << l << endl;
		return false;
	}

	/*thrid parameter (num marbles available)*/
	for (int i = 1; i < n; i++) {
		cp = cp->getAdjacent(fd);
		if (cp == nullptr) {
			//cout  << "Out of bounds, the number of marbles you are trying to move does not exist" << endl;
			return false;
		}

		if (marble != cp->getMarble()) {//confirm availability and type of claimed resources
			//cout << endl << "You cannot move the marble at " << cp->getLocation() << " (not your marble)" <<endl;
			return false;
		}
	}

	/*forth parameter (follow dir)*/
	if(inlineMove(n, fd, md)) {//in-line move
		cp = cells.at(l);//initialize to leading cell
		mtype = 0;
		//how many opponent marbles are in front of the leading marble i.e in the move direction
		for (int i = 0; i < n; i++) {
			cp = cp->getAdjacent(md);
			if (cp != nullptr) {//confirm availability of required resources
				if (marble == cp->getMarble()) {//met own marble instead of space
					//cout << "Cant move because your marble in the way at " << cp->getLocation() << endl;
					return false;
				}
				if (cp->getMarble() != '+')//if it belongs to opponent
					mtype++;//count opponent marbles
				else
					break;//reach empty space (+)
			}
			else {
				if (mtype > 0)//opponent's marble will be pushed off the board
					scoreMove = true;
				else {//if trying to push your own marble off the board
					//cout << "Cant move marbles of the board" << endl;
					return false;
				}
				break;
			}
		}
		if (mtype >= n) {//should only push fewer than n of the opponent's marbles
			//cout << "You dont have enough marbles to push the enemy" << endl;
			return false;
		}
	}
	else {//broad-side move
		mtype = -1;
		cp = cells.at(l);
		for (int i = 0; i < n; i++) {
			cell* dp = cp->getAdjacent(md);//destination pointer
			if (cp->getAdjacent(md) == nullptr) {
				//cout << "Cant move marbles of the board" << endl;
				return false;
			}
			if (dp->getMarble() != '+') {
				/*if a marble in the way*/
				//cout << "Cant move ( Marble " << dp->getMarble() << " is in way at " << dp->getLocation() << " )" << endl;
				return false;
			}
			cp = cp->getAdjacent(fd);
		}
	}
	return true;

}

bool board::executeMove(const char& m, const string& l, const int& n, const int& fd, const int& md) {
	int moveType = 0;//default to in-line with no following marbles
	bool scoreMove = false;
	bool valid = validateMove(m, l, n, fd, md, moveType, scoreMove);
	if (valid) {
		if (moveType == -1) {//broad-side
			cell* cp = cells.at(l);
			for (int i = 0; i < n; i++) {
				cell* dp = cp->getAdjacent(md);//destination pointer
				dp->setMarble(cp->getMarble());
				cp->setMarble('+');
				cp = cp->getAdjacent(fd);
			}
		}
		else {//in-line
			cell* cp = cells.at(l), * rearp = cp;
			const char marble = cp->getMarble();
			cell* frontp = cp->getAdjacent(md);
			for (int i = 1; i < n; i++) {
				rearp = rearp->getAdjacent(fd);
			}
			if (scoreMove) {
				frontp->setMarble(rearp->getMarble());
				rearp->setMarble('+');
				if (marble == 'O')
					this->boc++;
				if (marble == '@')
					this->woc++;
			}
			else {
				cell* tipp = frontp;
				for (int i = 0; i < moveType; i++) {
					tipp = tipp->getAdjacent(md);
				}
				tipp->setMarble(frontp->getMarble());
				frontp->setMarble(rearp->getMarble());
				rearp->setMarble('+');
			}
		}
	}
	else {
		cout << "Move: " << m << ", " << l << ", " << n << ", " << fd << ", " << md << endl;
	}
	return valid;
}

bool board::inPlay() {
	if (this->getBoc() == 6 || this->getWoc() == 6) {
		if (this->getBoc() == 6)
			cout << "Winner is White" << endl;
		else
			cout << "Winner is Black" <<endl;
		return false;
	}
		
	else
		return true;
}