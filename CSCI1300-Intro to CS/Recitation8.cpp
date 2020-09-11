# include <iostream>

using namespace std;

int main ()
{
    double myDoubles [20];
    for (int i = 0; i < sizeof(myDoubles) / sizeof(double); i++)
    {
        myDoubles[i] = i;
    }
    for (int i = 0; i < sizeof(myDoubles) / sizeof(double); i++)
    {
        cout << myDoubles[i] << endl;
    }
    double sum = 0;
    for (int i = 0; i < sizeof (myDoubles) / sizeof (double); i++)
    {
        sum = sum + myDoubles[i];
    }
    cout << sum / (sizeof(myDoubles) / sizeof (double)) << endl;
    return 0;
}
