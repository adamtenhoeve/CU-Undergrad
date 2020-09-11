# include <iostream>
# include "CommunicationNetwork.h"
# include <sstream>
# include <fstream>

using namespace std;

int main (int argc, char *argv[])
{
    char *filename = argv[1];
    CommunicationNetwork CallMe;
    int answer;
    cout << "======Main Menu======" << endl;
    cout << "1. Build Network" << endl << "2. Print Network Path" << endl;
    cout << "3. Transmit Message Coast-To-Coast" << endl << "4. Add City" << endl << "5. Quit" << endl;
    cin >> answer;
    while (answer != 5)
    {
        if (answer == 1)
        {
            CallMe.buildNetwork();
        }
        if (answer == 2)
        {
            CallMe.printNetwork();
        }
        if (answer == 3)
        {
            CallMe.transmitMsg(filename);
        }
        if (answer == 4)
        {
            cin.clear();
            cin.ignore(100, '\n');
            string n, p;
            cout << "Enter a city name: " << endl;
            getline (cin, n);
            cout << "Enter a previous city name: " << endl;
            getline (cin, p);
            CallMe.addCity(p, n);
        }
        cout << "======Main Menu======" << endl;
        cout << "1. Build Network" << endl;
        cout << "2. Print Network Path" << endl;
        cout << "3. Transmit Message Coast-To-Coast" << endl;
        cout << "4. Add City" << endl;
        cout << "5. Quit" << endl;
        cin >> answer;
    }
    if(answer == 5)
    {
        cout << "Goodbye!" << endl;
    }

}
