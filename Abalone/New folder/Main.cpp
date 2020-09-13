/*code from https://stackoverflow.com/questions/4193185/c-and-getasynckeystate-function and
  https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
  was modified to read keyboard input during run time to facilate pausing the game*/


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>
#include "agent.h"
#include "myAgent.h"

using namespace std;

int timeDelay = 1;//time between each move
void sleep(int&);
void displayMenu();//display option for when game is paused
void saveGame(const char&, const board&); //save board state to textfile if user chooses to save progress
bool checkPrevGameData();//check if there is saved data of previous game
void loadPrevGame(const board&);//if user chooses to finih game, load it
void exitGame(const board&);//exits game, calls the save game method to determine if to save game progress or not

int main(int argc, char** argv) {

	time_t now = time(NULL);
	srand(now);
	cout << "Seed: " << now << endl;

	//before start, check if a previous game was saved. If there is saved data, ask user if they choose to load it
	bool loadGame = checkPrevGameData();
	char finishGame = 'a';
	if (!loadGame) {
		cout << "No previous game data found" << endl;
		sleep(timeDelay);//wait 1sec to notify user no saved data found
	}
	else {
		while (finishGame != 'y' && finishGame != 'n') {
			cout << "Previous game data found. Finish game?(y/n) ";
			cin >> finishGame;
		}
	}

	//select mode
	cout << endl << "Please select game mode:" << endl;
	cout << "For CPU vs CPU, enter 1" << endl;
	cout << "For CPU vs Player, enter 2" << endl;
	cout << "For Player vs Player, enter 3" << endl;
	int mode;
	cin >> mode;
	
	if (mode == 3) {
		cout << "Press q then Enter to quit" << endl;
		sleep(timeDelay);
	}
	else {
		cout << endl << "Press ESC to pause" << endl;
		cout << "Press q then Enter to quit" << endl;
		cout << "Press ANY KEY then ENTER to unpause" << endl;
		sleep(timeDelay);
	}

	agent* w = nullptr;//CPU 1
	agent* b = nullptr;//CPU 2
	myAgent* player1 = nullptr;
	myAgent* player2 = nullptr;
	char playerMarble = 'a';
	if (mode == 1) {
		w = new randAgent('O');
		b = new randAgent('@');
	}
	else if (mode == 2) {
		while (playerMarble != 'O' && playerMarble != '@') {
			cout << "PLease select your marble (white - O ,black - @): ";
			cin >> playerMarble;
			if (playerMarble == 'O') {
				b = new distAgent('@');
				player1 = new myAgent('O');
			}
			else {
				w = new distAgent('O');
				player1 = new myAgent('@');
			}
		}
	}
	else if (mode == 3) {
		player1 = new myAgent('O');
		player2 = new myAgent('@');
		cout << "Player 1 is white (O) , Player 2 is Black (@)" << endl;
	}


	sleep(timeDelay);


	board abalone;
	//if user choose to finish prev game
	if (finishGame == 'y') {
		loadPrevGame(abalone);
	}

	string state(abalone);
	cout << endl << "Initial Board state:" << endl;
	cout << "Marbles Off Board: Black - " << abalone.getBoc() << "  White - " << abalone.getWoc() << endl << endl;
	cout << state;

	movement* pom = nullptr;
	char c = 'O';
	int moveNum = 0;//display for keeping track of number of moves made

	while (abalone.inPlay()) {

		//keep checking if escape button is pressed
		if (GetAsyncKeyState(VK_ESCAPE)) {//8000 for caps, 0001 for small letters
			cout << endl << "Game Paused" << endl;
			cout << "Press q then Enter to quit" << endl;
			cout << "Press ANY KEY then ENTER to unpause" << endl;
			char choice;
			cin >> choice;
			if (choice == 'q') {
				exitGame(abalone);
				return 0;
			}
			else {
				cout << endl << "Game unpaused" << endl;
				sleep(timeDelay);
			}
		}

		//wait 1 second before next turn
		sleep(timeDelay);

		cout << endl<< c<<"'s turn " << endl;
		try {
			if (c == 'O') {
				movement mm;
				if (mode == 1) {//CPU - generate random move for white marble
					 mm = w->move(pom, abalone);//get all the moves
					if (pom)//if pom is not empty,delete the move of previous player move to prevent memory leaks
						delete pom;
				}
				else if (mode == 2 && player1->getMarble() == '@') {//if player chose black marble, CPU must call move method, i.e. w.move
					//cout << "o turn ode 2 player @" << endl;
					mm = w->move(pom, abalone);
					if (pom)
						delete pom;
				}
				else if (mode == 2  && player1->getMarble() == 'O') {//if player chose white marble, call player MOVE method, i.e. player.move
					//cout << "O turn Mode 2 player O" << endl;
					 mm = player1->move(pom, abalone);
					if (pom)
						delete pom;
				}
				else {//mode 3, player MOVE methods always called
					 mm = player1->move(pom, abalone);
					if (pom)
						delete pom;	
				}
				pom = new movement(mm);
			}
			else {
				movement mm;
				if (mode == 1) {//CPU is playing so call MOVE b.move
					mm = b->move(pom, abalone);
					if (pom)
						delete pom;
				}
				else if (mode == 2 && player1->getMarble() == 'O') {//if player is white, CPU must call MOVE method for black marble, i.e. b.move
					//cout << "@ turn Mode 2 player O" << endl;
					 mm = b->move(pom, abalone);
					if (pom)
						delete pom;
				}
				else if (mode == 2  && player1->getMarble() == '@') {//if player chose white black, call player MOVE method, i.e. player.move
					//cout << "@ turn Mode 2 player B" << endl;
					 mm = player1->move(pom, abalone);//in CPU vs player, player 1 is default pointer
					if (pom)
						delete pom;
				}
				else {//mode 3, player2 MOVE methods always called
					 mm = player2->move(pom, abalone);
					if (pom)
						delete pom;	
				}
				pom = new movement(mm);
			}
		}
		catch (const string& s) {
			cout << s;
			return 1;
		}

		//if mode 3, check if user entered 'q' to exit game
		if (pom->l.find("q") == 0) {
			exitGame(abalone);
			return 0;
		}

		abalone.executeMove(c, pom->l, pom->n, pom->fd, pom->md);
		string state(abalone);
		cout <<  "Move " << moveNum + 1 << ": " << c << "," << pom->l << "," << pom->n << "," << pom->fd << "," << pom->md << endl;
		cout << "Next State:" << endl << state;
		moveNum++;

		//next player's turn
		if (c == 'O')
			c = '@';
		else
			c = 'O';

	}
	delete w;
	delete b;
	delete player1;
	delete player2;

	//string l; int n, fd, md; char m;
	//string winner;
	//bool gameOver = false;
	//while (!gameOver) {
	//	cout << "Enter Move: ";
	//	cin >> m >> l >> n >> fd >> md;
	//	if (m == 'q')return 0;//exit game
	//	bool valid = abalone.executeMove(m, l, n, fd, md);
	//	while (!valid) {
	//		cout << "Re-Enter Move: ";
	//		cin >> m >> l >> n >> fd >> md;
	//		if (m == 'q') return 0;//exit game
	//		valid = abalone.executeMove(m, l, n, fd, md);
	//	}
	//	string nbs(abalone);
	//	cout << endl << endl << "Next Abalone board state:" << endl;
	//	cout << "Marblse Off Board: Black - " << abalone.getBoc() << "  White - " << abalone.getWoc() << endl << endl;
	//	cout << nbs ;
	//	//turn is valid and executed, change playerTurn variable to next player
	//	if (abalone.getTurn() == 'O')
	//		abalone.setTurn('@');
	//	else
	//		abalone.setTurn('O');
	//	if (abalone.getWoc() == 6 || abalone.getBoc() == 6) {
	//		gameOver = true;
	//		if (abalone.getBoc() == 6) winner = "White Won!";
	//		else winner = "Black Won!";
	//		cout << "Game Over" << endl << winner << endl;
	//	}
	//}
	return 0;
}

void sleep(int& seconds) {
	int miliseconds = seconds * 1000;
	Sleep(miliseconds);
}


void displayMenu() {
	cout << endl << "Press ESC to pause" << endl;
	cout << "Press q then Enter to quit" << endl;
	cout << "Press ANY KEY then ENTER to unpause" << endl;
}


void saveGame(const char& saveGameChoice, const board& b) {

	if (saveGameChoice != 'y') {
		ofstream myfile;
		myfile.open("savedGame.txt");
		myfile << "Game Saved = false \n";
		myfile.close();
	}
	else {
		ofstream myfile;
		myfile.open("savedGame.txt");
		if (myfile.is_open()) {
			myfile << "Game Saved  = true\n";

			map<string, cell*> copy = b.getCells();
			map<string, cell*>::const_iterator it = copy.begin();
			while (it != copy.end()) {
				myfile << it->first << " - " << it->second->getMarble() << endl;
				it++;
			}

			cout << "Game saved";
			myfile.close();
		}
		else {
			cout << "Error: Could not save game";
		}
		myfile.close();
	}

}


bool checkPrevGameData() {
	string line;
	boolean savedData = false;
	// Read from the text file
	ifstream myFile("savedGame.txt");

	getline(myFile, line);
	if (line.find("true") != std::string::npos)
		savedData = true;
	else
		savedData = false;

	// Close the file
	myFile.close();
	return savedData;
}


void loadPrevGame(const board& b) {
	ifstream myfile;
	myfile.open("savedGame.txt");

	map<string, cell*> copy = b.getCells();
	string line;
	getline(myfile, line);//store first line which contains the check for whether games was saved or not
	while (getline(myfile, line)) {
		string l = line.substr(0, 2);
		char marble = line.substr(5, 6)[0];
		map<string, cell*>::const_iterator it = copy.find(l);
		cell* cp = it->second;
		cp->setMarble(marble);
	}

	myfile.close();
}

void exitGame(const board& b) {
	char saveGameChoice = 'a';
	while (saveGameChoice != 'y' && saveGameChoice != 'n') {
		cout << "Save game? (y/n) ";
		cin >> saveGameChoice;
	}
	saveGame(saveGameChoice, b);//call method which determines whether to save game to textfile or not
}