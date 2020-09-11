# include <iostream>
using namespace std;

int functionPower (float M, float M0, float V, float CF) // Where the total power output per second is calculated.
{
    float Pair = 0.18 * CF * V * V * V;
    float Proll = 0.001 * 9.8 * (M + M0) * V;
    int Psec = Pair + Proll;
    cout << Psec << endl;
    return Psec;
}

int functionWatts (float M, float M0, float V) // Where power changes depending on different CFdraft values.
{
    int CF = 3;
    int intPower = 0;
    while (CF < 10)
    {
        int intAir = 0.18 * CF / 10 * V * V * V;
        int intRoll = 0.001 * 9.8 * (M + M0) * V;
        intPower = intRoll = intAir;
        cout << intPower << endl;
        CF = CF + 1;
    }
    return intPower;
}

int funcitonTravel1 (float V, float Psec, int D, float M, float M0, float CF) // Energy total for entered value.
    {
        int intAir = 0.18 * CF * V * V * V;
        int intRoll = 0.001 * 9.8 * (M + M0) * V;
        int intPower = intRoll = intAir;
        int timeTravel =  (D * 1000) / V;
        int energyTotal;
        int energyTotalEntered = intPower * timeTravel;
        cout << energyTotalEntered << endl;
    }

int functionTravel2 (float V, float Psec, int D, float M, float M0) // Where total time and total energy are calculated for each CF.
{
    int CF = 3, intPower = 0;
    while (CF < 10)
    {
        int intAir = 0.18 * CF / 10 * V * V * V;
        int intRoll = 0.001 * 9.8 * (M + M0) * V;
        intPower = intRoll = intAir;
        CF = CF + 1;
        int timeTravel = (D * 1000) / V;
        int energyTotal;
        energyTotal = intPower * timeTravel;
        cout << energyTotal << endl;
    }
}

int main ()
{
    float M = 0.0, M0 = 0.0, V = 0.0, CFint = 0.0, Psec = 0.0;
    int intPower = 0, energyTotal = 0, D = 0, enteredEnergy = 0;
    cout << "Enter the mass of the rider in kg." << endl;
    cin >> M;
    cout << "Enter the mass of the bike in kg." << endl;
    cin >> M0;
    cout << "Enter the velocity." << endl;
    cin >> V;
    cout << "Enter a CF draft value" << endl;
    cin >> CFint;
    Psec = functionPower (M, M0, V, CFint);
    intPower = functionWatts (M, M0, V);
    cout << "Enter a distance in km." << endl;
    cin >> D;
    enteredEnergy = funcitonTravel1 (V, Psec, D, M, M0, CFint);
    energyTotal = functionTravel2 (V, Psec, D, M, M0);
}
