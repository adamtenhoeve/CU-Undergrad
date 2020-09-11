# include <iostream>
# include <fstream>
# include <iomanip>
using namespace std;

int Reading (ifstream & Readme)
{
    string line;
    getline(Readme, line);
    int x = stoi (line);
    return x;
}

int Writing (int n)
{
    int total = 0;
    if (n % 2 == 0)
    {
        total = n;
    }
    if ((n % 5 == 0) && (n % 10 != 0))
    {
        total = n;
    }
    return total;
}
int main ()
{
    ifstream Readme;
    ofstream Editme;
    string line;
    int sum = 0;
    Readme.open("InputFile_Recitation6.txt");
    Editme.open("OutputFile_Recitation6.txt");
    while (!Readme.eof())
    {
        int lineNumber = Reading(Readme);
        int correctN = Writing(lineNumber);
        sum = sum + correctN;
    }
    Editme << sum << "\n";
    Readme.close();
    Editme.close();
    return 0;
}

