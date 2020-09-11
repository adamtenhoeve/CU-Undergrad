// Adam Ten Hoeve
// 106105239
// CSCI 2270-110

# include <iostream>
# include <fstream>
# include <iomanip>
# include <string>
# include <stdlib.h>
using namespace std;

struct items // Like a class but smaller
{
	string name = ""; // Initializes the type of item, whether it's for sale or wanted, and its price for each struct.
	int sale = -1;
	int price = 0;

};

void EndArray (items buylist[100], int arraycount){ // a funciton that goes through the remaining array and outputs each item that has not been matched.
    for (int i = 0; i < arraycount; i++) // Goes through every item in the array
    {
        if (buylist[i].sale == 0) // For the output, checks whether each item is for sale or wanted.
        {
            cout << buylist[i].name << ", for sale, " << buylist[i].price << endl;
        }
        else
        {
            cout << buylist[i].name << ", wanted, " << buylist[i].price << endl;
        }
    }
}

int main () // Where all of the fun stuff happens
{
    items buylist[100]; // Creates a blank array of 100 structs
    string filename;
    cin >> filename; // Lets the user choose what filename is selected to be read in.
    int arraycount = 0; // An array count that increments depending on how many things are in the array.
	ifstream ReadMe;
	ReadMe.open(filename); // Opens the filename that the user inputed.
	if (!ReadMe.is_open()) // Checks to make sure the file is actually open.
	{
		cout << "Did not open" << endl;
	}


	string line;
	int read = 0, searches = 0, shifts = 0, matches = 0;
	for (int i = 0; !ReadMe.eof(); i++) // For each line in the file, reads that line and begins breaking it up into its parts until the end of the file.
	{
		string nName, nSale;
		getline(ReadMe, line); // Gets the next line in the file
		read++; // Iterates up to count the number of lines read.
		int c1 = line.find(','); // Finds the first comma so can seperate the type part of the file line.
        nName = line.substr(0, c1); // Sets a string to the first section of the line.

		int c2 = line.find(',', c1 + 1); // Finds the second comma
		nSale = line.substr(c1 + 2, c2 - c1 - 1); // Creates a string for the "for sale" or "wanted" section.
		int Buying = -1;
		if (nSale == "for sale,") // If "for sale" is the string, then sets the int sale of the struct to 0.
		{
			Buying = 0;
		}
		else // Sets the int to 1 if "wanted".
		{
			Buying = 1;
		}

		string tres = line.substr(c2 + 2, line.length() - c2 + 2); // Gets the third part of the line.
		int new_price = stoi(tres); // Converts the string to an int.

        bool match = false; // Creates a bool that will change to true if the current item has no matches currently in the array.

		for (int n = 0; n < arraycount; n++) // Compares the current item with each item currently in the array to see if there are any matches.
		{
		    searches++; // Each time it compares the item to a new item in the array, increments through a counter to count searches.
			if (nName == buylist[n].name) // Items must be of the same name.
			{
				if ((Buying == 0) && (buylist[n].sale == 1)) // If an item is being sold, sees if there are any buys willing to pay more than their price.
				{
					if (new_price <= buylist[n].price) // If all matching conditions are met, then removes the item in the array that was matched with it.
					{
					    matches++;
					    cout << nName << " " << new_price << endl;
						match = true; // Sets match equal to true so doesn't get added to the array.
						for (int j = n; j < arraycount; j++) // To remove an item, shift all items behind it up one to cover up its position.
						{
                            buylist[j] = buylist[j + 1]; // Starting with the matched item, copies the item to its right ontop of it until all items have been shifted over.
                            shifts++; // Each time an item moves, the shifts counter goes up by one.
						}
						shifts = shifts - 1; // Don't want to count the last shift of a blank variable so removes 1 from the counter each time it shifts and item out.
                        arraycount = arraycount - 1; // Iterates down through the array because an item has been shifted out.
                        break; // Ends the matching loop once a match has been found.
					}
				}
				if ((Buying == 1) && (buylist[n].sale == 0)) // Same as the last part but for "wanted" being compared to sellers who are selling for less than the buying price.
				{
					if (new_price >= buylist[n].price)
					{
					    matches++;
                        cout << nName << " " << buylist[n].price << endl;
						match = true;
						for (int j = n; j < arraycount; j++)
						{
                            buylist[j] = buylist[j + 1];
                            shifts++;
                        }
                        shifts = shifts - 1;
                        arraycount = arraycount - 1;
                        break;
					}
				}
			}
		}
        if (match == false) // If no matches have been found, adds the item to the array.
        {
            buylist[arraycount].name = nName; // Adds the name, sale, and price to each struct in the array at the current last position of the array.
            buylist[arraycount].sale = Buying;
            buylist[arraycount].price = new_price;
            arraycount++;
        }
	}
	ReadMe.close(); // Closes the file.
	cout << "#" << endl;
	EndArray(buylist, arraycount);
	cout << "#" << endl; // A bunch of cout statements.
	cout << "file read:" << read << endl;
	cout << "search array:" << searches << endl;
	cout << "shift array:" << shifts << endl;
}
