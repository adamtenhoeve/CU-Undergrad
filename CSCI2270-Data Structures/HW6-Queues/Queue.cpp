# include <iostream>
# include "Queue.h"

using namespace std;

Queue::Queue(int arraysize)
{
    queueSize = arraysize;
    queueHead = 0;
    queueTail = 0;
    queueCount = 0;
    arrayQueue = new string [queueSize];
}

Queue::~Queue()
{

}

void Queue::enqueue(string word)
{
    if (queueIsFull() == false)
    {


        arrayQueue[queueTail] = word;
        queueTail++;
        queueCount++;
        if (queueTail == queueSize)
        {
            queueTail = 0;
        }

        /*
        if (queueTail == queueSize - 1)
        {
            queueTail = 0;
            arrayQueue[queueTail] = word;
            queueCount++;
        }
        else
        {
            arrayQueue[queueTail] = word;
            queueTail++;
            queueCount++;
        } */
        cout << "E: " << word << endl;
        cout << "H: " << queueHead << endl;
        cout << "T: " << queueTail << endl;
    }
    else
    {
        cout << "Queue is full." << endl;
    }
}

string Queue::dequeue()
{
    if (queueIsEmpty() == false)
    {
        string word = arrayQueue[queueHead];
        queueHead++;
        if (queueHead == queueSize)
        {
            queueHead = 0;
        }
        queueCount--;
        cout << "H: " << queueHead << endl;
        cout << "T: " << queueTail << endl;
        cout << "word: " << word << endl;
        return word;
    }
    else
    {
        cout << "Queue is empty." << endl;
    }
    return "";
}

void Queue::printQueue()
{
    if (queueIsEmpty() == false)
    {
        int i = queueHead;
        cout << i << ": " << arrayQueue[i] << endl;
        i++;
        while (i != queueTail)
        {
            cout << i << ": " << arrayQueue[i] << endl;
            i++;
            if (i == queueSize)
            {
                i = 0;
            }
        }
    }
    else
    {
        cout << "Empty" << endl;
    }
}

bool Queue::queueIsFull()
{
    if (queueCount == queueSize)
    {
        return true;
    }
    return false;
}

bool Queue::queueIsEmpty()
{
    if (queueCount == 0)
    {
        return true;
    }
    return false;
}
