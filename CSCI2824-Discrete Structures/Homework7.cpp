# include <iostream>
using namespace std;

void seiveOfE(long int length)
{
  bool sieve[length + 1];
  for (long int i = 0; i < length + 1; i++)
  {
    sieve[i] = true; // Sets all numbers to true. If not prime, then will change to false.
  }
  for (long int i = 2; i < length + 1; i++)
  {
    if (sieve[i] == true) // only cares if the number has already been proven prime
    {
      for (long int j = 2; j * i < length + 1; j++)
      {
        sieve[i*j] = false; // Removes all numbers that are multiples of the prime number;
      }
    }
  }
  long long int total = 0;
  long int count = 0;
  for (int k = 2; k < length + 1; k++)
  {
    if (sieve[k] == true)
    {
      total = total + k;
      count++;
    }
  }
  cout << "For N = " << length << " there were " << count << " prime numbers with the total of " << total << "." << endl;
}

int main ()
{
  seiveOfE(10);
  seiveOfE(500000);
}
