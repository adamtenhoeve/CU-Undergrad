# include <iostream>
# include <fstream>
# include <iomanip>
# include <string>
using namespace std;

int howManyLines (string fileName) // Part 1
{
    ifstream Scores;
    string line;
    int lineCount = 0;
    Scores.open(fileName.c_str());
    if (Scores.fail())
    {
        cout << "File failed to open." << endl;
        return -1;
    }
    while (!Scores.eof()) // Goes through every line until hits the end of the file.
    {
        Scores >> line;
        lineCount++;
    }
    cout << "Number of lines: " << setw (7) << lineCount << endl;
    Scores.close();
    return lineCount;
}

int countMatchingRecords (string userfile, int low, int high) // Part 2
{
    string line;
    ifstream AllScores;
    AllScores.open (userfile);
    int matches = 0, examscores = 0;
    while (getline(AllScores, line))
    {
        getline(AllScores, line);
        getline(AllScores, line); // Goes through every third line to fine line with scores.
        int x = stoi (line);
        if ((high >= x) && (low <= x))
        {
            matches = matches + 1; // If score is between high and low, adds to counter.
        }
    }
    AllScores.close();
    cout << "Number of records: " << setw (7) << matches << endl;
    return matches;
}

string gradeFromScore (int grade) // Part 3 helper function
{
    string letter;
    if ((grade <= 100) && (grade >= 90))
    {
        letter = "A";
    }
    else if ((grade < 90) && (grade >= 80))
    {
        letter = "B";
    }
    else if ((grade < 80) && (grade >= 70))
    {
        letter = "C";
    }
    else if ((grade < 70) && (grade >= 60))
    {
        letter = "D";
    }
    else
    {
        letter = "F";
    }
    return letter; // All of the letters
}

int convertToLetterGrade (string infile, string outfile) // Part 3
{
    int totalScores = 0;
    string  firstName, lastName, score;
    ifstream Readme;
    Readme.open(infile);
    ofstream Editme;
    Editme.open(outfile);
    while (!Readme.eof())
    {
        getline (Readme, firstName); // Gets each line and puts it into its own string.
        getline (Readme, lastName);
        getline (Readme, score);
        int n = stoi (score);
        string grade = gradeFromScore(n);
        Editme << lastName << ", " << firstName << "\n";
        Editme << score << "\n";
        Editme << grade << "\n"; // Puts those strings in the right order into the output file.
        totalScores++;
    }
    Readme.close();
    Editme.close();
    cout << "Number of Grades: " << setw (7) << totalScores << endl;
}

int main ()
{
    // Part 1
    string file1;
    cout << "Enter a file:" << endl;
    cin >> file1;
    int nLines = howManyLines(file1);
    // Part 2
    string file2;
    int low, high;
    cout << "Enter a filename:" << endl;
    cin >> file2;
    cout << "Enter a low score:" << endl;
    cin >> low;
    cout << "Enter a high score:" << endl;
    cin >> high;
    int matches = countMatchingRecords (file2, low, high);
    // Part 3
    string inFile;
    string outFile;
    cout << "Enter input filename:" << endl;
    cin >> inFile;
    cout << "Enter output filename:" << endl;
    cin >> outFile;
    int records = convertToLetterGrade(inFile, outFile);
    return 0;
}
