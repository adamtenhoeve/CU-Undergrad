// Adam Ten Hoeve
// CSCI 3010 HW1
// Methods for the Player class

#include <iostream>
#include "Player.h"

using namespace std;

// Constructor for the player. Initial position set to (0,0).
Player::Player(const string name, const bool is_human)
{
	name_ = name;
	is_human_ = is_human;
	points_ = 0;
	pos_.row = 0;
	pos_.col = 0;
}

// Increase the current points by amount passed in.
void Player::ChangePoints(const int x)
{
	points_ += x;
}

// Change the current position of the player to the new location
void Player::SetPosition(Position pos)
{
	pos_.row = pos.row;
	pos_.col = pos.col;
}