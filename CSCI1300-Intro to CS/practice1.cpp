# include <iostream>
using namespace std;

void countLetter (string sentence, char letter)
{
    int index, number = 0;
    for (index = 0; index < (int)sentence.length (); index++)
    {
        if (sentence[index] == letter)
        {
            number = number + 1;
        }
    }
    cout << number;
}

int main ()
{
    string sentence;
    char letter;
    cout << "input a sentence" << endl;
    cin >> sentence;
    cout << "character" << endl;
    cin >> letter;
    countLetter (sentence, letter);
}
