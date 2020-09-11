# include <iostream>
# include <fstream>
# include <iomanip>
using namespace std;

int main ()
{
    ifstream Readme;
    ofstream Editme;
    Readme.open("InputFile_Recitation6.txt");
    Editme.open("OutputFile_Recitation6.txt");
    string line;
    int sum = 0;
    while (getline(Readme, line))
    {
        int n = stoi (line);
        if (n % 2 == 0)
        {
            Editme << line << "\n";
            sum = sum + n;
        }
        if ((n % 5 == 0) && (n % 10 != 0))
        {
            Editme << line << "\n";
            sum = sum + n;
        }
    }
    Editme << "The sum of all multiples of 2 and 5 is " << sum << "\n";
    Readme.close();
    Editme.close();
    return 0;
}
