#include <iostream>
#include "battleShip.h"

using namespace std;

int main()
{
	//Declare 3 instances of the battleship class: Destroyer-0 Carrier-0 Cruiser-0
    battleShip Destroyer ("Destroyer");
    battleShip Carrier ("Carrier");
    battleShip Cruiser ("Cruiser");

	//Give the ships a size: Destroyer-3 Carrier-10 Cruiser-2
	Destroyer.setSize (3);
	Carrier.setSize (10);
	Cruiser.setSize (2);

	// The while loop that runs until all of the ships are sunk. Does not go over the amount of hits needed to sink the boats.
	while((Destroyer.isSunk() == false) || (Carrier.isSunk() == false) || (Cruiser.isSunk() == false))
    {
        if (Destroyer.isSunk() == false)
        {
            Destroyer.recordHit();
        }
        if (Carrier.isSunk() == false)
        {
            Carrier.recordHit();
        }
        if (Cruiser.isSunk() == false)
        {
            Cruiser.recordHit();
        }
	}
	return 0;
}
