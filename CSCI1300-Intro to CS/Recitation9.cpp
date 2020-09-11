# include <iostream>
# include <fstream>

using namespace std;

int main ()
{
    ifstream Readme;
    Readme.open("Recitation9_Input.txt");
    if(!Readme.is_open())
    {
        cout << "File didn't open" << endl;
    }
    string line;
    int nums[29];
    for(int i = 0; i < 29; i++)
    {
        getline(Readme, line);
        int n = 0;
        n = stoi(line);
        nums[i] = n;
    }
    for (int i = 0; i < 29; i++)
    {
        int maxAmount = nums[i];
        int maxIndex = i;
        for (int j = i + 1; j < 29; j++)
        {
            if (nums[j] > maxAmount)
            {
                maxAmount = nums[j];
                maxIndex = j;
            }
        }
        int temp = nums[i];
        nums[i] = nums[maxIndex];
        nums[maxIndex] = temp;
    }
    for(int i = 0; i < 29; i++)
    {
        cout << nums[i] << endl;
    }
}
