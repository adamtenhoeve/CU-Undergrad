//Adam Ten Hoeve
//Emily Webb
//Final Project

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "Hash.h"
using namespace std;

int main(int argc, char*argv[])
{
   int hashSize = atoi(argv[1]);
   HashTable H = HashTable(hashSize);
   string change, line, First, Last;


   cout << "Hash Table Size: " << H.tableSize << endl;
   cout << "Collisions using open addressing: " << H.OpenCollisions << endl;
   cout << "Collisions using chaining: " << H.ChainCollisions << endl;
   cout << "Search operations using open addressing: " << H.OpenSearches << endl;
   cout << "Search operations using chaining: " << H.ChainSearches << endl;
   int input = 0;

   while (input != 2)
   {
     cout << "======Main Menu======"<<endl;
     cout << "1. Query hash table" << endl;
     cout << "2. Quit" << endl;
     cin >> input;
     if (input == 1)
     {
       cin.clear();
       cin.ignore(100,'\n');
       cout << "Enter player's first name: " << endl;
       getline(cin, First);
       cout << "Enter player's last name: " << endl;
       getline(cin, Last);
       string key = First + Last;
       H.FindPlayer(key);
     }
     /*
     if (input == 3)
     {
       H.countLines(); // Used to count the number of lines used in the chaining hash table
     }
     */
   }
   cout << "goodbye!" << endl;
}
