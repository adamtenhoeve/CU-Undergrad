# include <iostream>
using namespace std;

void calcMultiples (int n)
{
    int index, sum = 0;
    for (index = 1; index < n; index++)
    {
        if (index == 2) // So the output isn't a "+" at the very beginning.
        {
            cout << "2";
            sum = sum + 2;
        }
        if ((index % 2 == 0) && (index > 2)) // Finds all integers of 2 that go into the value, excluding 2 because that was used in the beginning.
        {
            cout << "+" << index;
            sum = sum + index;
        }
        if ((index % 5 == 0) && (index % 10 != 0)) // Finds every multiple of 5 but skips over the multiples of 10.
        {
            cout << "+" << index;
            sum = sum + index;
        }
    }
    cout << "=" << sum << endl;
}

int main ()
{
    int n;
    cout << "Input a number" << endl;
    cin >> n;
    calcMultiples (n);

}
