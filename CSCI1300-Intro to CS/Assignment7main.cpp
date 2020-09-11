# include <iostream>
# include <fstream>

# include "BikeRacer.h"

using namespace std;

int main ()
{
    // ------- Part 1 --------

    // ------- Part 2 --------
    Racer Steve;
    Steve.setBikeMass (15);
    Steve.setBikeCR (0.001);
    Steve.setCDraft (0.70);
    Steve.setK (0.18);
    Steve.setVelocity (10.8);
    Steve.setRiderMass (63.0);
    float power = Steve.Power ();
    cout << power << endl;
}
