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

HashTable::HashTable(int size)
{
  tableSize = size;
  ChainCollisions = 0;
  ChainSearches = 0;
  OpenCollisions = 0;
  OpenSearches = 0;
   string line;
   string str_yearID;
   int yearID; //year between 1985 and 2016
   string teamID; //three-letter abriviation for team
   string leagueID; // national league (NL) or american league (AL)
   string playerID; // ID for the player
   string str_salary;
   int salary; //how much money player made that year
   string firstName;
   string lastName;
   string key = firstName+lastName;
   string str_birthYear;
   int birthYear;
   string birthCountry;
   string str_weight;
   int weight;
   string str_height;
   int height;
   string bats; //bats with right (R) or left (L) or switches between the two(S)
   string throws; //throws right(R) or throws left (L)
   chainHT = new Player *[tableSize];
   openHT = new Player *[tableSize];
   for(int i = 0; i < size; i++)
   {
      chainHT[i] = NULL;
      openHT[i] = NULL;
   }
   //*******
   ifstream FH;
   FH.open("playerData.txt");
   if(!FH.is_open())
   {
      cout << "File could not open." << endl;
      return;
   }
   stringstream ss;
   getline(FH, line); //read in first line
   ss << line;
   while(getline(FH, line))
   {
      stringstream ss2;
      ss2 << line;
      getline(ss2, str_yearID, ',');
      yearID = stoi(str_yearID);
      getline(ss2, teamID, ',');
      getline(ss2, leagueID, ',');
      getline(ss2, playerID, ',');
      getline(ss2, str_salary, ',');
      salary = stoi(str_salary);
      getline(ss2, firstName, ',');
      getline(ss2, lastName, ',');
      getline(ss2, str_birthYear, ',');
      birthYear = stoi(str_birthYear);
      getline(ss2, birthCountry, ',');
      getline(ss2, str_weight, ',');
      weight = stoi(str_weight);
      getline(ss2, str_height, ',');
      height = stoi(str_height);
      getline(ss2, bats, ',');
      getline(ss2, throws, ',');
      Player *p1 = new Player(yearID, teamID, leagueID, playerID, salary, firstName,
         lastName, birthYear, birthCountry, weight, height, bats, throws);
       Player *p2 = new Player(yearID, teamID, leagueID, playerID, salary, firstName,
          lastName, birthYear, birthCountry, weight, height, bats, throws);
      hashTableChain(p1, size); //chaining
      hashTableOpen(p2, size); //open addressing
   }
   FH.close();

}
HashTable::~HashTable()
{

}

int HashTable::hashSum(string key, int size)
{
   int sum = 0;
   for(int i = 0; i < key.length(); i++)
   {
      sum = sum + key[i];
   }
   sum = sum % size;
   return sum;
}

void HashTable::hashTableChain(Player *p, int BigSize)
{
   string key = p->key;
   int index = hashSum(key, tableSize);
   if(chainHT[index] != NULL) //traverse through the linked list
   {

      ChainCollisions++;
      Player *temp = chainHT[index]; //the player that is already there
      Player *parent = NULL;
      while(temp != NULL && temp -> key <= p -> key)
      {
        ChainSearches++;
        parent = temp;
        if(p->key == temp->key && p -> birthYear == temp -> birthYear) //player already exists
        {
          ChainCollisions--;
          temp -> yearArray[temp -> arrayCounter] = p -> yearID;
          temp -> teamArray[temp -> arrayCounter] = p -> teamID;
          temp -> leagueArray[temp -> arrayCounter] = p -> leagueID;
          temp -> salaryArray[temp -> arrayCounter] = p -> salary;
          temp -> arrayCounter++;
          return;
        }
        temp = temp -> next;
      }
      if (temp == NULL) //add to tail
      {
        parent -> next = p;
      }
      else if (parent == NULL) //add to head
      {
        p -> next = temp;
        chainHT[index] = p; //makes  it the head of linked list
      }
      else //add in middle
      {
        p -> next = temp;
        parent -> next = p;
      }

      //if there is something already there
   }
   else
   {
      chainHT[index] = p;

      //just add it as the first element in the linked list at that position
   }
}

void HashTable::hashTableOpen(Player *p, int size)
{
    int index = hashSum(p -> key, tableSize);
    if (openHT[index] != NULL)
    {
      if(p -> key == openHT[index] -> key && p -> birthYear == openHT[index] -> birthYear)
      {
        Player *temp = openHT[index];
        temp -> yearArray[temp -> arrayCounter] = temp -> yearID;
        temp -> teamArray[temp -> arrayCounter] = temp -> teamID;
        temp -> leagueArray[temp -> arrayCounter] = temp -> leagueID;
        temp -> salaryArray[temp -> arrayCounter] = temp -> salary;
        temp -> arrayCounter++;
        return;
      }
      OpenCollisions++;
      int i = index;
      i++;
      while (openHT[i] != NULL && i != index)
      {
        OpenSearches++;
        if(p -> key == openHT[i] -> key && p -> birthYear == openHT[i] -> birthYear)
        {
          Player *temp = openHT[i];
          OpenCollisions--;
          temp -> yearArray[temp -> arrayCounter] = temp -> yearID;
          temp -> teamArray[temp -> arrayCounter] = temp -> teamID;
          temp -> leagueArray[temp -> arrayCounter] = temp -> leagueID;
          temp -> salaryArray[temp -> arrayCounter] = temp -> salary;
          temp -> arrayCounter++;
          return;
        }
        if (i + 1 == tableSize)
        {
          i = 0;
        }
        else
        {
          i++;
        }
      }
      openHT[i] = p;
    }
    else
    {
      openHT[index] = p;
    }
}

void HashTable::FindPlayer(string name)
{
  int index = hashSum(name, tableSize);
  int openSearches = 0, chainSearches = 0;
  ////////// Chain Search ///////////////
  Player *temp = chainHT[index];
  if (temp == NULL)
  {
    cout << "Player not found" << endl;
    return;
  }
  while(temp != NULL && temp -> key != name)
  {
    chainSearches++;
    temp = temp -> next;
  }
  if (temp == NULL)
  {
    cout << "Player not found" << endl;
    return;
  }
  ////////// Open Address Search ////////
  Player *temp2 = openHT[index];
  int i = index + 1;
  while (temp2 -> key != name && i != index)
  {
    openSearches++;
    temp2 = openHT[i];
    if (i + 1 == tableSize)
    {
      i = 0;
    }
    else
    {
      i++;
    }
  }
///////////////////////////////////////
  if (temp == NULL)
  {
    cout << "Player not found" << endl;
    return;
  }
  else
  {
    cout << "First name: " << temp -> firstName << endl;
    cout << "Last name: " << temp -> lastName << endl;
    cout << "Year Born: " << temp -> birthYear << endl;
    cout << "Country Born: " << temp -> birthCountry << endl;
    cout << "Weight: " << temp -> weight << endl;
    cout << "Height: " << temp -> height << endl;
    cout << "Bats: " << temp -> bats << endl;
    cout << "Throws: " << temp -> throws << endl;
    cout << "Teams and Salary: " << endl;
    for (int i = 0; i < temp -> arrayCounter; i++)
    {
      cout << temp -> yearArray[i] << ", " << temp -> teamArray[i];
      cout << ", " << temp -> leagueArray[i] << ", " << temp -> salaryArray[i] << endl;
    }
    cout << "Search operations using open addressing: " << openSearches << endl;
    cout << "Search operations using chainging: " << chainSearches << endl;
  }
}

/*
void HashTable::countLines()
{
  int count = 0;
  for (int i = 0; i < 5147; i++)
  {
    if (chainHT[i] != NULL)
    {
      count++;
    }
  }
  cout << count << endl;
}
*/
