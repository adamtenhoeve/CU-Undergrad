#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
# include <iostream>
# include "Card.h"

using namespace std;

class Player
{
public: // Data Members
    Card Hand[10];
    int handSize;
    int handValue;
    int SpareChange;

public: // Access Methods
    Player();

    void sethandSize();
    int gethandSize();

    void setChangeWin(int bet);
    void setChangeLoss(int bet);
    int getChange();

    int calcValue ();

    void Clear();

};


#endif // PLAYER_H_INCLUDED
