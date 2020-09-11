#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

typedef unsigned long long int Adamsnum;

unsigned long long int findFactor(unsigned long long int n)
{

  unsigned long long int m = sqrt(n);
  if (n % 2 == 0)
  {
    return 1;
  }
  for (unsigned long long int i = 3; i < m + 1; i = i + 2)
  {
    if (i % 2 == 0 && i != 2)
    {
      continue;
    }
    if (n % i == 0)
    {
      return i;
    }
  }
  return 1;
}

// void Euclidian(unsigned long long int e, unsigned long long int pq, unsigned long long int &x, unsigned long long int &y)
// {
//   if (e % pq == 0)
//   {
//     x = 0;
//     y = 1;
//     return;
//   }
//   Euclidian(pq, e % pq, x, y);
//   unsigned long long int temp = x;
//   x = y;
//   y = temp - y * (e / pq);
// }
//
// unsigned long long int inverse(unsigned long long int e, unsigned long long int pq)
// {
//   unsigned long long int x,y;
//   x = 0;
//   y = 0;
//   Euclidian(e, pq, x, y);
//   if (x != 0)
//   {
//     x = x + pq;
//   }
//   return x;
// }

int main(int argc, char const *argv[])
{
  //__uint128_t n = cstrtoint128(argv[1]);
  //unsigned long long int n = 10868018840696929921;
  //unsigned long long int n = 402127908109; // Message 1;
  //unsigned long long int e = 134042213173; // Message 1;
  //unsigned long long int n = 4866515299480983357750662903; Message 3
  //unsigned long long int e = 1622171766493612990803004177; Message 3

  unsigned long long int p = findFactor(n);
  unsigned long long int q = n / p;
  unsigned long long int j = (p-1)*(q-1);
  //cout << "Inverse: " << inverse(e, j) << endl;
  cout << "p is " << p << " and q is " << q << endl;
  cout << "(p-1)*(q-1) is " << j << endl;
}
