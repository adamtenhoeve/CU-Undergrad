# include <iostream>
# include "Dealer.h"

using namespace std;

Dealer::Dealer ()
{
    handSize = 0;
    handValue = 0;
}

void Dealer::sethandSize ()
{
    handSize++;
}

int Dealer::gethandSize()
{
    return handSize;
}

int Dealer::calcValue()
{
    handValue = 0;
    cout << "The hand size is " << handSize << endl;
    for(int i = 0; i < handSize; i++)
    {
        handValue = handValue + Hand[i].getAmount();
    }
    return handValue;
}
