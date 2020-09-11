# include <iostream>
# include "player.h"

using namespace std;

Player::Player()
{
    handSize = 0;
    handValue = 0;
    SpareChange = 500;
}

void Player::sethandSize()
{
    handSize++;
}

int Player::gethandSize()
{
    return handSize;
}

void Player::setChangeWin(int money)
{
    SpareChange = SpareChange + money;
}

void Player::setChangeLoss(int money2)
{
    SpareChange = SpareChange - money2;
}

int Player::getChange()
{
    return SpareChange;
}

int Player::calcValue()
{
    handValue = 0;
    for (int i = 0; i < handSize; i++)
    {
        handValue = handValue + Hand[i].getAmount();
    }
    return handValue;
}

void Player::Clear()
{
    handSize = 0;
    handValue = 0;
    for (int i = 0; i < 10; i++)
    {
        Hand[i].setAmount(0);
        Hand[i].setFace("");
        Hand[i].setSortAmount(0);
        Hand[i].setSuit("");
    }
}
