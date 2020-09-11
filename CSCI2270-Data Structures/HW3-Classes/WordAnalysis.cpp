# include <iostream>
# include <fstream>
# include <sstream>
# include "WordAnalysis.h"

using namespace std;


WordAnalysis::WordAnalysis(int ish) // The constructor for the class;
{
    timesDoubled = 0;
    wordCount = ish;
    index = 0;
    words = new word[ish]; // Sets both the wordCount and the initial arraysize to the entered variable.
}

WordAnalysis::~WordAnalysis() // DESTRUCTOR
{
    delete []words;
}

void WordAnalysis::doubleArrayAndAdd (string newWord)
{
    if (index == wordCount - 1) // If statement to check to see if needs to be doubled
    {
        wordCount = wordCount * 2;
        word *newarray = new word[wordCount]; // Creates new array that's twice the size and copies old array over to it.
        for (int i = 0; i < wordCount/2; i++)
        {
            newarray[i] = words[i];
        }
        delete []words;
        words = newarray;
        timesDoubled++;
    }

    bool common = checkIfCommonWord(newWord); // Checks the common words
    if ((common == false) and (newWord.compare("") != 0))
    {
        bool found = false;
        for (int n = 0; n < index; n++) // If word is already in the array, then increment the count by one.
        {
            if (newWord == words[n].w)
            {
                found = true;
                words[n].count++;
                break;
            }
        }
        if (found == false) // If the word is not in the array, adds the word, sets the count to one, and increases the index by one.
        {
            words[index].w = newWord;
            words[index].count = 1;
            index++;
        }
    }
}

bool WordAnalysis::checkIfCommonWord(string newWord) // Compares words to the 50 most common words. If the word is not a match, returns false so the program can keep going with the word.
{
    string commons[50] = {"the", "be", "to", "of", "and", "a", "in", "that", "have", "i", "it", "for", "not", "on", "with", "he", "as", "you", "do", "at", "this", "but", "his", "by", "from", "they", "we", "say", "her", "she", "or", "an", "will", "my", "one", "all", "would", "their", "there", "what", "so", "up", "out", "if", "about", "who", "get", "which", "go", "me"};
    for (int i = 0; i < 50; i++)
    {
        if (newWord == commons[i])
        {
            return true;
        }
    }
    return false;
}

void WordAnalysis::sortData() // Insertion sort to sort the items with the largest counts to the front of the array.
{
    for (int i = 1; i < index; i++)
    {
        word temp;
        temp = words[i];
        int j = i;
        while ((j > 0) && (words[j - 1].count < temp.count))
        {
            words[j] = words[j - 1];
            j--;
        }
        words[j] = temp;
    }
}

bool WordAnalysis::readDataFile(char* fn) // Reading in the data file. If the file opens, then begins the array-making process.
{
    string filename = fn;
    ifstream ReadMe;
    ReadMe.open(filename);
    if (!ReadMe.is_open())
    {
        return false; // Returns false if did not open so file doesn't try to do other things.
    }

    string line;
    while (getline (ReadMe, line)) // Getline to get each line from the ifstream of the text file.
    {
        string word;
        stringstream bitline;
        bitline << line;
        while (getline(bitline, word, ' ')) // Getline to get each word from the line.
        {
            word.erase(word.find_last_not_of(" \n\r\t") + 1);
            doubleArrayAndAdd(word);
        }
    }
    return true; // Returns true once all of the array has been made.
}

int WordAnalysis::getWordCount() // Returns the total number of noncommon words seen by the array.
{
    int j = 0;
    for (int i = 0; i < index; i++)
    {
        j = j + words[i].count;
    }
    return j;
}

int WordAnalysis::getUniqueWordCount() // Returns the number of individual words in the array, also the array length.
{
    return index;
}

int WordAnalysis::getArrayDoubling() // returns how many times the array had to double to contain all of the words.
{
    return timesDoubled;
}

void WordAnalysis::printCommonWords(int n) // Calls the sort function and outputs the first n words, where n is the input variable.
{
    sortData();
    for (int i = 0; i < n; i++)
    {
        cout << words[i].count << " - " << words[i].w << endl;
    }
}
