# include <iostream>

using namespace std;

int main ()
{
    string Cards [52];
    for (int i = 0; i < sizeof(Cards)/sizeof(string); i++)
    {
        for (j = 0; j < sizeof(Cards)/sizeof(string), j++)
        {

        }
        if (i % 4 == 0)
        {
            cout << " of Hearts" << endl;
        }
        if (i % 4 == 1)
        {
            cout << " of Spades" << endl;
        }
        if (i % 4 == 2)
        {
            cout << " of Diamonds" << endl;
        }
        if (i % 4 == 3)
        {
            cout << "of Clubs" << endl;
        }
    }
}
