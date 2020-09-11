#include <iostream>
using namespace std;
int Problem1 ()
{
    int currentPopulation;
        currentPopulation = 370,357,870;
    int time = 31536000;
    int births = time/7;
    int deaths = time/13;
    int immigrants = time/35;
    int estimatedPopulation = 0;
    estimatedPopulation = currentPopulation - deaths + births + immigrants;
    cout << "The estimated population in one year is " << estimatedPopulation << "." << endl;
    return 1;
}

int timeCalc (int seconds)
{
    int X = 0, Y = 0, Z = 0;
    int timeLeft = seconds;
    while (timeLeft >= 3600)
    {
        X = X + 1;
        timeLeft = timeLeft - 3600;
    }
    while (timeLeft >= 60)
    {
        Y = Y + 1;
        timeLeft = timeLeft - 60;
    }
    while (timeLeft >= 1)
    {
        Z = Z + 1;
        timeLeft = timeLeft - 1;
    }
    cout << "The time is " << X << "hours, " << Y << "minutes, and " << Z << "seconds." << endl;
    return 2;
}

int Problem2 ()
{
    int seconds = 0;
    cout << "Enter a number of seconds between 0 and 86400." << endl;
    cin >> seconds;
    if (seconds <= 0)
    {
        cout << "Invalid entry. Please put in a number between 0 and 86400" << endl;
        cin >> seconds;
    }
    else if (seconds > 86400)
    {
        cout << "Invalid entry. Please put in a number between 0 and 86400" << endl;
        cin >> seconds;
    }
    int endResult = timeCalc (seconds);
    if (endResult = 2)
        return 2;

}

int Problem3 ()
{
    int Fahrenheit;
    cout << "Enter a fahrenheit temperature." << endl;
    cin >> Fahrenheit;
    int Fahrenheit2 = Fahrenheit - 32;
    int Fahrenheit3 = Fahrenheit2 *5/9;
    int Celcius = Fahrenheit3;
    cout << Fahrenheit << " degrees fahrenheit is " << Celcius << " degrees celcius." << endl;
    return 3;
}

int main ()
{
    int V1 = Problem1 ();
    if (V1 == 1)
    {
        int V2 = Problem2 ();
        if (V2 == 2)
        {
            int V3 = Problem3 ();
            if (V3 == 3)
            {
                cout << "End of problems." << endl;
            }
        }
    }
}
