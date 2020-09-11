# include <iostream>
# include <fstream>
# include <iomanip>
# include <string>
using namespace std;


int howManyLines(string fileName) // Part 1
{
    int iFile = std::stoi(fileName);
    ifstream Scores;
    Scores.open("/home/user/Desktop/StudentScores1.txt");
    if (Scores.fail())
    {
        cout << "Scores1 failed to open" << endl;
        return -1;
    }
    int lineCount = 0;
    while (std::getline(Scores,filename))
    {
        lineCount = lineCount + 1;
    }
    Scores.close ();
    cout << "Number of lines: " << setw(7) << lineCount << endl;
    return lineCount;
}

int main ()
{
    string file;
    cout << "Enter a filename:" << endl;
    cin >> file;
    int nFiles = howManyLines(file);

}
