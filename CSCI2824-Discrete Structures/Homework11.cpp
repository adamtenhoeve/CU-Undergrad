#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

// Code written by Adam Ten Hoeve and Erin Ruby

void Monty(bool switchDoors, float trials)
{
  float wins = 0;
  for (int i = 0; i < trials; i++)
  {
    int car = rand() % 3 + 1;
    //cout << "car is " << car << endl;
    int choice = rand() % 3 + 1;
    //cout << "choice is " << choice << endl;
    int doorOpen = rand() % 3 + 1;
    while (doorOpen == car || doorOpen == choice)
    {
      doorOpen = rand() % 3 + 1;
    }
    //cout << "doorOpen is " << doorOpen << endl;
    int doorNotOpen = 6 - doorOpen - choice;
    //cout << "doorNotOpen is " << doorNotOpen << endl;
    if (switchDoors == true)
    {
      choice = doorNotOpen;
      //cout << "after switch, choice is " << choice << endl;
    }
    if (car == choice)
    {
      wins++;
    }
  }
  if (switchDoors == true)
  {
    cout << "When switching doors everytime, ";
  }
  else
  {
    cout << "When never switching doors, ";
  }
  cout << "won " << wins << " times and won " << wins/trials * 100 << "% of the time." << endl;
}

int main()
{
  srand(time(NULL));
  Monty(true, 100);
  Monty(true, 1000);
  Monty(true, 10000);
  Monty(true, 100000);

  Monty(false, 100);
  Monty(false, 1000);
  Monty(false, 10000);
  Monty(false, 100000);
}
