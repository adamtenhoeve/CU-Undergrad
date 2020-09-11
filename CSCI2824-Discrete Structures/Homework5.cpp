# include <iostream>
# include <math.h>

using namespace std;

/* Adam Ten Hoeve
    106105239
    CSCI 2824*/

    // Code for the summation of all even Lucas numbers.
int evenLucasSum (int n)
{
    int total = 2;
    int first = 2;
    int second = 1;
    for (int i = 0; i < n-1; i++)
    {
        int c = first + second;
        if (c % 2 == 0)
        {
            total = total + c;
        }
        first = second;
        second = c;
    }
    return total;
}
    // Code for finding the first Lucas number of that many digits.
int firstDDigitLucas(int digits)
{
    if (digits == 1)
    {
        return 2;
    }

    long int lucas0 = 2;
    long int lucas1 = 1;
    long int c;

    long int found = pow(10, digits-1);

    int i = 1;
    while ((lucas1 / found) < 1)
    {
        c = lucas0 + lucas1;
        lucas0 = lucas1;
        lucas1 = c;
        i++;
    }

    cout << "The first number with " << digits << " digits is " << lucas1 << " at the location " << i << "." << endl;
}

int main ()
{   // ------------   Part 1    ------------------------------
    int N = 50;
    int x = evenLucasSum(N);
    cout << "The total of all the evens of the first " << N << "+1 Lucas Numbers is " << x << "." << endl;
    //-------------   Part 2    ------------------------------

    int D = 15;
    long int firstNumber = firstDDigitLucas(D);



}
