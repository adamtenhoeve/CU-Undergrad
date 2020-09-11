#include <iostream>
#include "battleShip.h"

using namespace std;

battleShip::~battleShip () // Initial constructor for the class if no parameters are passed to it.
{
    name = "Boaty McBoatface";
    size = 0;
    hits = 0;
}

battleShip::battleShip (string boatName) // Constructor for when the name is passed as a constructor
{
    name = boatName;
    size = 0;
    hits = 0;
}

void battleShip::setShipName (string new_name) // Sets and gets for the name of the ship.
{
    name = new_name;
}

string battleShip::getShipName ()
{
    return name;
}

void battleShip::setSize (int new_size) // Sets and gets for the size of the ship.
{
    size = new_size;
}

int battleShip::getSize ()
{
    return size;
}

void battleShip::recordHit() // If there is a hit on the ship then the total number of hits goes up by one.
{
    hits++;
}

bool battleShip::isSunk() // Checks the number of hits to the length to see if the ship is sunk.
{
    if (size == hits)
        return true;
    return false;
}
