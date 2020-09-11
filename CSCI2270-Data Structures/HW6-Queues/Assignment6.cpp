# include <iostream>
# include "Queue.h"
# include <sstream>

using namespace std;

int main()
{
    Queue queueMe(10);
    int answer = 0;
    while (answer != 5)
    {
        cout << "======Main Menu=====" << endl;
        cout << "1. Enqueue word" << endl;
        cout << "2. Dequeue word" << endl;
        cout << "3. Print queue" << endl;
        cout << "4. Enqueue sentence" << endl;
        cout << "5. Quit" << endl;
        cin >> answer;
        if (answer == 1)
        {
            string inWord;
            cout << "word: ";
            cin >> inWord;
            queueMe.enqueue(inWord);
        }
        if (answer == 2)
        {
            string wordy = queueMe.dequeue();
        }
        if (answer == 3)
        {
            queueMe.printQueue();
        }
        if (answer == 4)
        {
            cin.clear();
            cin.ignore(100, '\n');
            string sentence;
            cout << "sentence: ";
            getline (cin, sentence);
            stringstream ss;
            ss << sentence;
            string word;
            while (getline(ss, word, ' '))
            {
                queueMe.enqueue(word);
            }
        }
    }
    cout << "Goodbye!" << endl;
}
