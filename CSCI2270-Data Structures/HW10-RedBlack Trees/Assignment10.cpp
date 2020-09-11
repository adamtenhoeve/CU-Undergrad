#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "MovieTree.h"
using namespace std;

int main(int argc, char *argv[])
{
  MovieTree WatchMe;
  string filename = argv[1];

  ifstream ReadMe;
  ReadMe.open(filename);
  if (!ReadMe.is_open())
  {
    cout << "File did not open" << endl;
    return 0;
  }
  string line, rank, title, year, amount;
  while (getline(ReadMe, line))
  {
    stringstream ss;
    ss << line;
    getline(ss, rank, ',');
    int ranking = stoi(rank);
    getline(ss, title, ',');
    getline(ss, year, ',');
    int releaseYear = stoi(year);
    getline(ss, amount);
    int quantity = stoi(amount);
    WatchMe.addMovieNode(ranking, title, releaseYear, quantity);
  }
  int choice = 0;
  while (choice != 7)
  {
    cout << "======Main Menu======" << endl;
    cout << "1. Find a movie" << endl;
    cout << "2. Rent a movie" << endl;
    cout << "3. Print the inventory" << endl;
    cout << "4. Delete a movie" << endl;
    cout << "5. Count the movies" << endl;
    cout << "6. Count the longest path" << endl;
    cout << "7. Quit" << endl;
    cin >> choice;
    if (choice == 1)
    {
      cin.clear();
      cin.ignore(100, '\n');
      string movie;
      cout << "Enter title:" << endl;
      getline (cin, movie);
      WatchMe.findMovie(movie);
    }
    else if (choice == 2)
    {
      cin.clear();
      cin.ignore(100, '\n');
      string movie;
      cout << "Enter title:" << endl;
      getline (cin, movie);
      WatchMe.rentMovie(movie);
    }
    else if (choice == 3)
    {
      WatchMe.printMovieInventory();
    }
    else if (choice == 4)
    {
      cin.clear();
      cin.ignore(100, '\n');
      string movie;
      cout << "Enter title:" << endl;
      getline (cin, movie);
      WatchMe.deleteMovieNode(movie);
    }
    else if (choice == 5)
    {
      cout<<"Tree contains: " << WatchMe.countMovieNodes() << " movies." << endl;
    }
    else if (choice == 6)
    {
      cout << "Longest Path: " << WatchMe.countLongestPath() << endl;
    }
  }
  cout << "Goodbye!" << endl;
}
