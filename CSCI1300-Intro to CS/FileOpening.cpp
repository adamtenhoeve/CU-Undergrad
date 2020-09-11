# include <iostream>
# include <fstream>
using namespace std;

int main ()
{
    ofstream dataFile;
    dataFile.open("testfile.txt", ios::out);
    if (dataFile.fail())
    {
        cout << "File open error!" << endl;
        return -1;
    }
    cout << "File was created successfully.\n";
    cout << "Now closing the file.\n";
    dataFile << "Jones\n";
    dataFile << "Smith\n";
    dataFile << "Willis\n";
    dataFile << "Davis\n";
    dataFile.close();
    cout << "Done.\n";
    dataFile.close();
    return 0;
}
