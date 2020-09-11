#ifndef DEALER_H_INCLUDED
#define DEALER_H_INCLUDED

# include <iostream>
# include "Card.h"

using namespace std;

class Dealer
{
    public: // Data Members
        Card Hand[10];
        int handSize;
        int handValue;
    public: // Access Methods

        Dealer();

        void sethandSize();
        int gethandSize();

        int calcValue();
};

#endif // DEALER_H_INCLUDED
