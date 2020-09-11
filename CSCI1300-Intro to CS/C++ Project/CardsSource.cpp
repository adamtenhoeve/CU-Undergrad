# include <iostream>

# include "Card.h"

using namespace std;

Card::Card()
{
    Suit = "";
    Face = "";
    amount = 0;
    sort_amount = 0;
}

Card::Card(string letter, string name, int n, int k)
{
    Suit = letter;
    Face = name;
    amount = n;
    sort_amount = k;
}

void Card::setSuit (string new_suit)
{
    Suit = new_suit;
}

string Card::getSuit()
{
    return Suit;
}

void Card::setFace (string new_face)
{
    Face = new_face;
}

string Card::getFace()
{
    return Face;
}

void Card::setAmount (int newN)
{
     amount = newN;
}

int Card::getAmount ()
{
    return amount;
}

void Card::setSortAmount (int k)
{
    sort_amount = k;
}

int Card::getSortAmount ()
{
    return sort_amount;
}
