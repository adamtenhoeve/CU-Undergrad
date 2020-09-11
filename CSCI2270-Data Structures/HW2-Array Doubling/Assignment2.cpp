// Adam Ten Hoeve 106105239 CSCI 2270-110
# include <iostream>
# include <fstream>
# include <string>
# include <iomanip>
# include <sstream>
using namespace std;
struct foundWords // Struct for each word in the novel and how many times it shows up.
{
    int counter;
    string name;
    foundWords ()
    {
        counter = 0;
        name = "";
    }
};

bool checkCommon (string word) // Function to compare each word to the  most common words
{
    string commons[50] = {"the", "be", "to", "of", "and", "a", "in", "that", "have", "i", "it", "for", "not", "on", "with", "he", "as", "you", "do", "at", "this", "but", "his", "by", "from", "they", "we", "say", "her", "she", "or", "an", "will", "my", "one", "all", "would", "their", "there", "what", "so", "up", "out", "if", "about", "who", "get", "which", "go", "me"};
    for (int i = 0; i < 50; i++)
    {
        if (word == commons[i])
        {
            return true;
        }
    }
    return false;
}

int main (int argc, char* argv []) // Yay main! It has parameters now!
{
    //if (argc != 2)
    //{
    //    argc = 2;
    //    argv[1] = "HungerGames_edit.txt";
    //}
    int doubles = 0, totalwords = 0;
    int arraysize = 100;
    foundWords *strawberry = new foundWords [arraysize]; // An array of pointers that each point to a struct.
    int arraycount = 0;
    string filename;
    filename = argv[1]; // Filename is equal to the input at the time of execution.
    ifstream ReadMe;
    ReadMe.open(filename);
    if (!ReadMe.is_open())
    {
        cout << "Did not open" << endl;
    }

    string line = "";
    while (getline(ReadMe, line)) // A loop that gets each line from the book.
    {
        stringstream mango; // A stringstream that lets you break up the line at each space.
        mango << line;
        string word = "";
        while (getline(mango, word, ' ')) // A loop that takes each word from the line so it can be analyzed.
        {
            word.erase(word.find_last_not_of(" \n\r\t")+1); // Gets rid of the extra end-lines from the file so the words are properly analyzed.
            if (arraycount == arraysize - 1) // Tests to see if array doubling is needed.
            {
                arraysize = arraysize * 2;
                foundWords *mango = new foundWords[arraysize]; // Creates a pointer array that is twice as long as the last one.
                for(int i = 0; i < arraysize/2; i++) // Copies stuff over from the existing array.
                {
                mango[i].counter = strawberry[i].counter;
                mango[i].name = strawberry[i].name;
                }
                delete []strawberry; // THEY MAY TAKE OUR TIME, BUT THEY WILL NEVER TAKE OUR MEMORY!!!
                strawberry = mango;
                doubles++;
            }
            bool match = checkCommon(word); // Checks the word against all 50 of the common words and returns a bool of whether it was true or false.
            if ((match == false) && (word.compare("") != 0)) // If it was not a match to the common words and is not a space (because sometimes that happens), it gets compared to the current array.
            {
                bool found = false;
                for (int i = 0; i < arraycount; i++) // Compares the word to all the existing words in the array. If there is a match, increases the counter of that word by 1. Otherwise adds that word to the array.
                {
                    if (word == strawberry[i].name)
                    {
                        found = true;
                        strawberry[i].counter++;
                        break;
                    }
                }
                if (found == false)
                {
                    strawberry[arraycount].name = word;
                    strawberry[arraycount].counter++;
                    arraycount++;
                }
                totalwords++; // Incrememnts each time a word is seen. Because novels aren't big or anything.
            }
        }
    }
    int ntop; // Gets the number of output words wanted from the command line.
    if (argc == 2)
    {
        ntop = stoi(argv[2]);
    }
    else
    {
        ntop = 10;
    }

    string top = " ";
    for (int i = 1; i < arraycount; i++) // Insertion sort to move the things with the largest count of occurances of a word to the front and the least to the end.
    {
        foundWords temp;
        temp = strawberry[i];
        int j = i;
        while ((j > 0) && (strawberry[j - 1].counter < temp.counter))
        {
            strawberry[j] = strawberry[j - 1];
            j--;
        }
        strawberry[j].counter = temp.counter;
        strawberry[j].name = temp.name;
    }

    for (int n = 0; n < ntop; n++) // Outputs the top n words that were most occuring, as well as the number of times doubled, unique-uncommon words seen, and total non-common words seen.
    {
        cout << strawberry[n].counter << " - " << strawberry[n].name << endl;
    }
    cout << "#" << endl;
    cout << "Array doubled: " << doubles << endl;
    cout << "#" << endl;
    cout << "Unique non-common words: " << arraycount << endl;
    cout << "#" << endl;
    cout << "Total non-common words: " << totalwords << endl;

}
