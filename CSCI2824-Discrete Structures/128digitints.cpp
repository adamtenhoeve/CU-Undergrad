#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

__uint128_t cstrtoint128(char const *arg){
  __uint128_t tmp = 0;
  int len = strlen(arg);
  int cnv;
  for (int i = 0; i < len; i++) {
    cnv = arg[i] - 48; // Use ascii values to convert
    tmp += cnv;
    if (i != len - 1){ // Avoid integer overflow
      tmp *= 10;
    }
  }
  return tmp;
}

int main(int argc, char const *argv[]) {
  __uint128_t n = cstrtoint128(argv[1]); // Assumes number passed as command-line argument
  // cout << n << endl; // This won't work; can't cout by default
  uint64_t q = n % 2147483647; // This sort of thing will work, though
  cout << q << endl;
  return 0;
}
