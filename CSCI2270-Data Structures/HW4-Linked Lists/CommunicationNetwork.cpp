# include "CommunicationNetwork.h"
# include <iostream>
# include <fstream>
# include <sstream>

using namespace std;

CommunicationNetwork::CommunicationNetwork()
{

}

CommunicationNetwork::~CommunicationNetwork()
{

}

void CommunicationNetwork::addCity(string prev, string newcity)
{
   City *left = head;
   while (left -> cityName != prev)
   {
       left = left -> next;
   }
   City *n = new City(newcity, NULL, "");
   if (left == NULL)
   {
       n -> next = head;
       head = n;
   }
   else if (left -> next == NULL)
   {
       left -> next = n;
       tail = n;
   }
   else
   {
       n -> next = left -> next;
       left -> next = n;
   }
}

void CommunicationNetwork::buildNetwork()
{
    string cities[11] = {"Los Angeles", "Phoenix", "Denver", "Dallas", "St. Louis", "Chicago", "Atlanta", "Washington, D.C.", "New York", "Boston", ""};
    head = new City (cities[0], NULL, "");
    tail = new City (cities[0], NULL, "");

    City *current = head;
    City *n = NULL;
    while (current -> next != NULL)
    {
        n = current -> next;
        delete current;
        current = n;
    }


    City *LA = new City (cities[0], NULL, "");
    for (int i = 0; i < 9; i++)
    {
        addCity(cities[i], cities[i+1]);
    }
    printNetwork();
}

void CommunicationNetwork::printNetwork()
{
    cout << "===CURRENT PATH===" << endl;
    City *temp = head;
    while (temp != NULL)
    {
        cout << temp -> cityName << " -> ";
        temp = temp -> next;
    }
    cout << "NULL" << endl;
    cout << "==================" << endl;
}

void CommunicationNetwork::transmitMsg(char *filename)
{
    if (head == NULL)
    {
        cout << "Empty list" << endl;
    }
    string file = filename;
    ifstream ReadMe;
    ReadMe.open(file);
    if (!ReadMe.is_open())
    {
        cout << "Did not open" << endl;
    }
    string line;
    while (getline (ReadMe, line));
    {
        string word;
        stringstream ss;
        ss << line;
        while (getline(ss, word, ' '))
        {
            word.erase(word.find_last_not_of(" \n\r\t") +1);
            City *current = head;
            City *n = NULL;
            current -> message = word;
            while (current != NULL)
            {
                cout << current -> cityName << " received " << current -> message << endl;
                n = current -> next;
                if (n == NULL)
                {
                    break;
                }
                n -> message = current -> message;
                current -> message = "";
                current = n;
            }
        }
    }
}
