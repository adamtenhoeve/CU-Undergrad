#ifndef DECKOFCARDS_H_INCLUDED
#define DECKOFCARDS_H_INCLUDED

# include <iostream>
# include <cstdlib>
# include <ctime>

# include "Card.h"

using namespace std;

class Deck
{

public: // Data Members

    int topCard;
    int Size;
    Card cards [52];
    Card temp;

public: // Access Methods

    Deck ();

    void Shuffle ();
    void Unshuffle ();

};

#endif // DECKOFCARDS_H_INCLUDED
