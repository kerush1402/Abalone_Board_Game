#include "agent.h"
#include <iostream>
//struct movment
movement::movement(const string& ll, const int& nn, const int& f, const int& m, const int& d, const int& mt, const bool& sm) :
    l(ll), n(nn), fd(f), md(m), d2e(d), mtype(mt), scoreMove(sm) {}

movement::movement(){}

movement::movement(const movement& copy) : l(copy.l), n(copy.n), fd(copy.fd), md(copy.md), d2e(copy.d2e), mtype(copy.mtype), scoreMove(copy.scoreMove) {}

//class agent
agent::agent(const char& c) : symbol(c) {
    int i = 0;
    for (int n = 1; n < 4; n++) {//3 marbles participate in a move
        for (int fd = 0; fd < 6; fd++) {//all 6 directions
            if (n == 1) {//will create 6 moves for n=1
                vector<int> tv;
                tv.push_back(1);
                tv.push_back(fd);
                tv.push_back(getAdjacentIndex(fd));
                cout << "index " << i << 1 << " " << fd << " " << getAdjacentIndex(fd) <<endl;
                pmd.push_back(tv);
                i++;
            }
            else {//will create a total of 60 moves for n=2 and n=3
                for (int md = 0; md < 6; md++) {
                    if (md != fd) {
                        vector<int> tv;
                        tv.push_back(n);
                        tv.push_back(fd);
                        tv.push_back(md);
                        cout << "index " << i << " " <<n << " " << fd << " " << md <<endl;
                        pmd.push_back(tv);
                        i++;
                    }
                    
                }
            }
            
        }
    }
}

vector<movement> agent::findMoves(const board& bd) const {
    vector<movement> mv;//store all the possible mobes available for the current player this turn
    map<string, cell*> cells = bd.getCells();

    //loop through all the cells in the map
    for (map<string, cell*>::iterator it = cells.begin(); it != cells.end(); it++) {
        string l = it->first;
        cell* cp = it->second;
        //if the cell == current player turn, add possible move directions
        if (cp->getMarble() == symbol) {
            set<int> mdSet;//directions in which neighbours either have a space or an opponent marble
            for (int md = 0; md < 6; md++) {
                cell* np = cp->getAdjacent(md);
                if (np != nullptr && np->getMarble() != symbol) {//the neighbour isnt your marble, add the move to the set
                    mdSet.insert(md);
                }
            }

            //loop though all potential moves established in pmd, and check if any match the possible moves from the set
            for (vector<int> tv : pmd) {
                int md = tv[2];//if the moves in the set match any in pmd based on the the md
                if (mdSet.find(md) != mdSet.end()) {//if the move exists 
                    int mtype = 0;//default to in-line with no following marbles
                    bool scoreMove = false;

                    //                               n   , fd,  md
                    if (bd.validateMove(symbol, l, tv[0], tv[1], md, mtype, scoreMove))
                        mv.push_back(movement(l, tv[0], tv[1], md, cp->getDistanceToEdge(md), mtype, scoreMove));
                }
            }
        }
    }
    return mv;
}


//struct moveTNDPriority
double moveTNDPriority::score(const movement& a) const {
    double maxScore = 0;
    if (a.mtype < 0)//broad
        maxScore = 0.8;
    else if (a.mtype > 0)//inline
        maxScore = 1.0;
    else
        maxScore = 0.6;

    if (a.mtype > 0) {
        return maxScore * (1 + (1 - a.d2e) / 9.0);
    }
    else {
        return maxScore * (1 - abs(5 - a.d2e) / 5.0);
    }
}
bool moveTNDPriority::operator()(const movement& a, const movement& b) const {
    return score(a) > score(b);//best moves at the front
}


//class randAgent
randAgent::randAgent(const char& c) :agent(c) {}

movement randAgent::move(const movement* const om, board bd) {
    vector<movement> mv = findMoves(bd);//all possible moves
    int size = mv.size();
   
    /*for (int k = 0; k < size; k++) {
        movement m = mv.at(k);
        std::cout << "Possible move: " << m.l << ", " << m.n << ", " << m.fd << ", " << m.md << endl;
    }*/
    
    if (mv.size() == 0) {
        throw string("Error! There are no available moves!");
    }
    int i = rand() % mv.size();
    return mv[i];
}


//class distAgent
distAgent::distAgent(const char& c) :agent(c) {}

movement distAgent::move(movement const* const om, board bd) {
    vector<movement> mv = findMoves(bd);
    sort(mv.begin(), mv.end(), moveTNDPriority());
    int size = mv.size();
    /*for (int k = 0; k < size; k++) {
        movement m = mv.at(k);
        std::cout << "Possible move: " << m.l << ", " << m.n << ", " << m.fd << ", " << m.md << endl;
    }*/

    const int bm2c = 3;//consider only these top best moves.
    if (mv.size() == 0) {
        throw string("Error! There are no available moves!");
    }
    if (mv.size() < bm2c)
        return mv[0];
    else {
        int i = rand() % bm2c;
        return mv[i];
    }
}