#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

# include <iostream>
# include <fstream>

using namespace std;

class Card
{
    public: // Data Members
        string Suit;
        string Face;
        int amount;
        int sort_amount;

    public: // Access Methods
        Card ();
        Card (string, string, int, int);

        void setSuit (string);
        string getSuit ();

        void setFace (string);
        string getFace ();

        void setAmount (int);
        int getAmount ();

        void setSortAmount (int);
        int getSortAmount ();
};



#endif // CARD_H_INCLUDED
