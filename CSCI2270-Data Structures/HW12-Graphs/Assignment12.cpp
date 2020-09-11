#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Graph.h"

using namespace std;

int main(int argc, char *argv[]) // Remember to make this a command line at the end
{
  Graph<string> ConnectMe;
  ifstream ReadMe;
  ReadMe.open(argv[1]);
  if (!ReadMe.is_open())
  {
    cout << "File did not open" << endl;
    return -1;
  }
  string line, word;
  stringstream ss;
  getline(ReadMe, line);
  ss << line;
  getline(ss, word, ',');
  while (getline(ss, word, ','))
  {
    ConnectMe.addVertex(word);
  }
  string city;
  while (getline(ReadMe, line))
  {
    stringstream ss2;
    ss2 << line;
    getline(ss2, city, ',');
    int index = 0, edge = 0;
    while (getline(ss2, word, ','))
    {
      edge = stoi(word);
      if (edge > 0)
      {
        string out = ConnectMe.getVertex(index);
        ConnectMe.addEdge(city, out, edge);
      }
      index++;
    }
  }
  ReadMe.close();

  ///////////////////////////////////////////////////////////////////////////////////

  int choice = 0;
  while (choice != 4)
  {
    cout << "======Main Menu======" << endl;
    cout << "1. Print vertices" << endl;
    cout << "2. Find districts" << endl;
    cout << "3. Find shortest path" << endl;
    cout << "4. Quit" << endl;
    cin >> choice;

    if (choice == 1)
    {
      ConnectMe.printVerticies();
    }

    if (choice == 2)
    {
      ConnectMe.findDistricts();
    }
    if (choice == 3)
    {
      cin.clear();
      cin.ignore(100, '\n');
      string start, end;
      cout << "Enter a starting city:" << endl;
      getline(cin, start);
      cout << "Enter an ending city:" << endl;
      getline(cin, end);
      ConnectMe.shortestPath(start, end);
    }
  }
  cout << "Goodbye!" << endl;

}
