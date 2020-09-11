#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "ElectoralMap.h"
#include "Election.h"
#include "TextUI.h"

using namespace std;

// Name: Adam Ten Hoeve

int main()
{

	// Set a random seed so the numbers are different each time.
	srand(time(0));
	
	// Initialize a vector of all the parties in the ElectoralMap.
    vector<PartyType> parties = GetParties();

    // Initalize the textUI that will print to the screen.
	TextUI *ui = new TextUI();

	// Do not let them leave until they give a valid answer. Only breaks the loop with reponse "direct", "representative" or "0"
	while(1)
	{
		// Choose the type of election. Elec initalized outside of loop for scoping.
		Election *elec = new Election;
		string input = "";
		// Prompt the user to select a type of election.
		cout << "What kind of election should we have (direct or representative, 0 to stop)? ";
		cin >> input;

		// Check if want to stop. This is statement is why it's in main because I couldnt' figure out how to make it exit from the method.
		if (input == "0")
		{
			// Exit the function.
			return 0;
		}
		// If they want a direct election
		else if (input == "direct")
		{
			// Assign elec as the base election class. Already declared so don't have to do anything.
		}
		// If they want a representaive election.
		else if (input == "representative")
		{
			// If want representative election, then point the elec to new instance of elec as Representative class.
			Election *temp = new RepresentativeElection();
			elec = temp;
		}
		// Otherwise invalid input.
		else
		{
			cout << endl << "Invalid input. ";
			continue;
		}

		// Iterates through all the parties and let the user add candidates.
		ui -> AddCandidates(parties, elec);

		// Print the Districts and the constituents
		ui -> PrintDistricts(elec);

		// Print all the Candidates and their affiliated party
		ui -> PrintCandidates(elec);

		// Campaigning. User selects a candidate and a district, then are asked again. Press 0 to exit.
		ui -> Campaigning(elec);

		// Voting. Constituents vote for candidates based on their party affiliations.
		ui -> Voting(elec);
		// Election is over, so delete the election object
		delete(elec);
	}
	return 0;
}