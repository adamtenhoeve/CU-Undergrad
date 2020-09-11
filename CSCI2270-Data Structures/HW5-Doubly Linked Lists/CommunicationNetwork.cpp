# include <iostream>
# include <sstream>
# include <fstream>
# include "CommunicationNetwork.h"

using namespace std;

CommunicationNetwork::CommunicationNetwork()
{
    head = NULL;
    tail = NULL;
}

CommunicationNetwork::~CommunicationNetwork()
{

}

void CommunicationNetwork::addCity(string prev, string newcity)
{
    City *left = head;
    if (prev == "First")
    {
        City *n = new City(newcity, left, NULL, "");
        left -> previous = n;
        head = n;
        return;
    }
    while (left -> cityName != prev)
    {
        left = left -> next;
    }
    City *n = new City(newcity, NULL, NULL, "");
    if (left == NULL)
    {
        n -> next = head;
        head = n;
        head -> previous = NULL;
    }
    else if (left -> next == NULL)
    {
        left -> next = n;
        tail = n;
        n -> previous = left;
    }
    else
    {
        City *right = left -> next;
        right -> previous = n;
        n -> next = left -> next;
        left -> next = n;
        n -> previous = left;
    }
}


void CommunicationNetwork::buildNetwork()
{
    string cities[10] = {"Los Angeles", "Phoenix", "Denver", "Dallas", "St. Louis", "Chicago", "Atlanta", "Washington, D.C.", "New York", "Boston"};
    head = new City (cities[0], NULL, NULL, "");
    tail = new City (cities[0], NULL, NULL, "");

    City *current = head;
    City *n = NULL;
    while (current -> next != NULL)
    {
        n = current -> next;
        delete current;
        current = n;
    }

    City *LA = new City (cities[0], NULL, NULL, "");
    for (int i = 0; i < 9; i++)
    {
        addCity(cities[i], cities[i+1]);
    }
    printNetwork();
}

void CommunicationNetwork::transmitMsg(char *filename)
{
    cout << filename << endl;
    if (head == NULL)
    {
        cout << "Empty list" << endl;
    }
    ifstream ReadMe;
    ReadMe.open(filename);
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
            current = tail -> previous;
            current -> message = word;
            while (current != NULL)
            {
                cout << current -> cityName << " received " << current -> message << endl;
                n = current -> previous;
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

void CommunicationNetwork::printNetwork()
{
     cout << "===CURRENT PATH===" << endl;
    City *temp = head;
    while (temp != NULL)
    {
        if (temp == head)
        {
            cout << "NULL <- ";
        }
        cout << temp -> cityName;
        if (temp -> next == NULL)
        {
            cout << " -> ";
        }
        else
        {
            cout << " <-> ";
        }
        temp = temp -> next;
    }
    cout << "NULL" << endl;
    cout << "==================" << endl;
}

void CommunicationNetwork::deleteCity(string gone)
{
    City *temp = head;
    while (temp -> cityName != gone)
    {
        temp = temp -> next;
    }
    if (temp == head)
    {
        head = head -> next;
        head -> previous = NULL;
        delete temp;
    }
    else if (temp -> next == NULL)
    {
        tail = tail -> previous;
        tail -> next = NULL;
        delete temp;
    }
    else
    {
        temp -> previous -> next = temp -> next;
        temp -> next -> previous = temp -> previous;
        delete temp;
    }
}

void CommunicationNetwork::deleteNetwork()
{
    City *temp = head;
    City *x = NULL;
    while (temp != NULL)
    {
        cout << "deleting " << temp -> cityName << endl;
        x = temp -> next;
        delete temp;
        temp = x;
    }
    head = NULL;
    tail = NULL;
}
