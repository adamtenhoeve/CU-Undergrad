# include <iostream>
using namespace std;

void story ()
{
    string noun1, noun2, adj1, adj2;
    cout << "Enter a noun:" << endl;
    cin >> noun1;
    cout << "Enter an adjective:" << endl;
    cin >> adj1;
    cout << "Enter another noun:" << endl;
    cin >> noun2;
    cout << "Enter another adjective:" << endl;
    cin >> adj2;
    cout << "It was the " << adj1 << " of " << noun1 << ", it was the " << adj2 << " of " << noun2 << endl;
    cout << "Would you like to play again? (y) or (n)" << endl;
}

int main ()
{
    int x = 0;
    while (x < 1)
        char choice;
        cout << "Do you want to play a game? (y) or (n)" << endl;
        cin >> choice;
        if (choice == n)
        {
            cout << "Good bye." << endl;
            x = x + 1
        }
        else if (choice == y)
        {
            void story ();
        }
        else
        {
            cout << "Invalid response. Please enter (y) or (n)." << endl;
        }
}
