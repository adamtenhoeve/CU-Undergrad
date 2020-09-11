//Adaam Ten Hoeve
//Emily Webb
//Final Project

#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;
struct Team {
   int yearID; //year between 1985 and 2016
   string teamID; //three-letter abriviation for team
   string leagueID; // national league (NL) or american league (AL)
   int salary;
};
struct Player
{

   int yearID; //year between 1985 and 2016
   string teamID; //three-letter abriviation for team
   string leagueID; // national league (NL) or american league (AL)
   string playerID; // ID for the player
   int salary; //how much money player made that year
   string firstName;
   string lastName;
   string key;
   int birthYear;
   string birthCountry;
   int weight;
   int height;
   string bats; //bats with right (R) or left (L) or switches between the two(S)
   string throws; //throws right(R) or throws left (L)
	Player *next;
   int arrayCounter;
   int yearArray[25];
   string teamArray[25];
   string leagueArray[25];
   int salaryArray[25];

	Player(){};
	Player(int in_yearID, string in_teamID, string in_leagueID, string in_playerID, int in_salary, string in_firstName,
         string in_lastName, int in_birthYear, string in_birthCountry, int in_weight, int in_height, string in_bats, string in_throws)
   {
      yearID = in_yearID;
      teamID =  in_teamID;
      leagueID = in_leagueID;
      playerID = in_playerID;
      salary = in_salary;
      firstName = in_firstName;
      lastName = in_lastName;
      key = firstName+lastName;
      birthYear = in_birthYear;
      birthCountry = in_birthCountry;
      weight = in_weight;
      height = in_height;
      bats = in_bats;
      throws = in_throws;
		  next = NULL;
      arrayCounter = 1;
      yearArray[0] = in_yearID;
      teamArray[0] = in_teamID;
      leagueArray[0] = in_leagueID;
      salaryArray[0] = in_salary;

	}
};

class HashTable
{
	public:
    int tableSize;
    int ChainCollisions;
    int ChainSearches;
    int OpenCollisions;
    int OpenSearches;

		HashTable(int size = 100);
		~HashTable();
      void hashTableChain(Player *p, int size);
      void hashTableOpen(Player *p, int size);
      int hashSum(string x, int size);
      void FindPlayer(string);
      Player *search(string);
      //void countLines();
	private:
      Player **chainHT;
      Player **openHT;
};

#include "Hash.cpp"
#endif // HASH_H
