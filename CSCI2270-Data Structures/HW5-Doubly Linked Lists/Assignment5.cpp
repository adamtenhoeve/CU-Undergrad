# include <iostream>
# include "CommunicationNetwork.h"
# include <sstream>
# include <fstream>

using namespace std;

int main (int argc, char *argv[])
{
    char *filename = argv[1];
    CommunicationNetwork CallMe;
    string answer;
    cout << "======Main Menu======" << endl;
    cout << "1. Build Network" << endl;
    cout << "2. Print Network Path" << endl;
    cout << "3. Transmit Message Coast-To-Coast-To-Coast" << endl;
    cout << "4. Add City" << endl;
    cout << "5. Delete City" << endl;
    cout << "6. Clear Network" << endl;
    cout << "7. Quit" << endl;
    getline(cin, answer);
    while (answer != "7")
    {
        if (answer == "1")
        {
            CallMe.buildNetwork();
        }
        if (answer == "2")
        {
            CallMe.printNetwork();
        }
        if (answer == "3")
        {
            CallMe.transmitMsg(filename);
        }
        if (answer == "4")
        {
            string n, p;
            cout << "Enter a city name: " << endl;
            getline (cin, n);
            cout << "Enter a previous city name: " << endl;
            getline (cin, p);
            CallMe.addCity(p, n);
        }
        if (answer == "5")
        {
            string d;
            cout << "Enter a city name: " << endl;
            getline(cin, d);
            CallMe.deleteCity(d);
        }
        if (answer == "6")
        {
            CallMe.deleteNetwork();
        }
        cout << "======Main Menu======" << endl;
        cout << "1. Build Network" << endl;
        cout << "2. Print Network Path" << endl;
        cout << "3. Transmit Message Coast-To-Coast-To-Coast" << endl;
        cout << "4. Add City" << endl;
        cout << "5. Delete City" << endl;
        cout << "6. Clear Network" << endl;
        cout << "7. Quit" << endl;
        getline (cin, answer);
    }
    cout << "Goodbye!" << endl;
    CallMe.deleteNetwork();
}
