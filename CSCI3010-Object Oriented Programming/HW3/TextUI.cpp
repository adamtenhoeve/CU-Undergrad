#include <iostream>
#include "TextUI.h"

// Prompts the user to input candidates for each party. Has the option to input none or any number of candidates per party.
void TextUI::AddCandidates(vector<PartyType> parties, Election* elec)
{
	// Loop through all the parties and add candidates to each. Size - 1 because don't want to add candidates to party::None.
	for(unsigned i = 0; i < parties.size()-1; i++)
	{
		string input = "";
		// Continue to add candidates to that party until the user stops.
		while(input != "n")
		{
			cout << "Do you want to register a candidate for party " << StringifyPartyType(parties[i]) << " (y or n)? ";
			cin >> input;
			// If they would like to add a candidate, then add one to the election class
			if(input == "y")
			{
				string name;
				cout << "What is their name? ";
				cin >> name;
				elec -> AddCandidate(name, parties[i]);
			}
			// If they enter no, go to next party;
			else if(input == "n")
			{
				continue;
			}
			// Otherwise is an invalid input.
			else
			{
				cout << "Invalid input. ";
			}
		}
	}
}

void TextUI::PrintDistricts(Election *elec)
{
	// Return a vector of all the districts in the ElectoralMap
	vector<District*> districts = elec -> get_districts();
	// Get a vector of all the parties in the election
	vector<PartyType> parties = GetParties();

	// Loop over entire vector and print all the districts
	for (unsigned i = 0; i < districts.size(); i++)
	{
		cout << "District " << districts[i] -> get_id() << ":" << endl;
		cout << "square miles: " << districts[i] -> get_area() << endl;
		// Iterate over all the parties and print the number of constituents per party.
		for (unsigned j = 0; j < parties.size(); j++)
		{
			cout << "[Party: " << StringifyPartyType(parties[j]) << "] : " << districts[i] -> get_constituents()[j] << "\t";
		}
		cout << endl << endl;;
	}
	cout << "-----------------" << endl;
}

// Print all of the candidates in the election
void TextUI::PrintCandidates(Election *elec)
{
	// Get a vector of all the candidates
	vector<Candidate*> candidates = elec -> get_candidates();
	// For each candidate, print out their id, name and party affiliation.
	for(unsigned i = 0; i < candidates.size(); i++)
	{
		cout << candidates[i] -> id_ << ": " <<candidates[i] -> name_;
		cout << " [Party: " << StringifyPartyType(candidates[i] -> party_) << "]" << endl;
	}
	cout << endl << endl;
}

// Prompt the user to increase certain candidate's scores in certain districts. Continues to do so until user exits.
void TextUI::Campaigning(Election *elec)
{
	// The state determines which part of the loop to repeat. Whether to ask which candidate is campaigning or where.
	int state = 1;
	// Variables to store the user input and ids of which candidate and which district.
	int candID = 0;
	int distID = 0;
	string input = "";
	// Repeat the loop until the user inputs 1, which is checked at each input.
	while(1)
	{
		// In this state, ask the user which candidate is campaigning
		if (state == 1)
		{
			// Determine which candidate is campaigning
			cout << "Which candidate is campaigning (id) (0 to stop) ? ";
			cin >> input;
			cout << endl;
			// If user decides to quit, exit the function.
			if(input == "0") {return;}

			// User checking to make sure is a valid candidate id
			candID = stoi(input);
			if ((candID < 1 ) || (candID > elec -> get_candidates().size())) 
			{
				cout << "Invalid input. " << endl; 
				// Return to the original question.
				continue;
			}
			PrintDistricts(elec);
			state = 2;
		}
		// In this state, ask the user where the selected candidate is going.
		if (state == 2)
		{
			cout << "Where is this candidate campaigning (id) (0 to stop) ? ";
			cin >> input;
			cout << endl;

			// If user selects stop, send back to state 1 (which candidate to select).
			if (input == "0")
			{
				state = 1;
				continue;
			}

			// User checking to make sure is a valid district id
			distID = stoi(input);
			if ((distID < 1) || (distID > elec -> get_districts().size()))
			{
				cout << "Invalid input" << endl;
				// returns. State does not change so again asks for where chandidate is going.
				continue;
			}
			state = 1;
		}
		// Print the candidate's name and where they are going. Note that position in candidate and district vectors are [input ID] - 1.
		cout << elec -> get_candidates()[candID - 1] -> name_ << " is campaigning in district " << distID << endl;
		elec -> CandidateIsCampaigning(candID - 1, distID - 1);
		PrintDistricts(elec);
	}
}

// Constituents vote for candidates based on party affiliations (or randomly).
void TextUI::Voting(Election *elec)
{
	// Get the final vote tallies from the election class.
	vector<int> finalVotes = elec -> Votes();
	cout << endl << "Final Results: " << endl;
	// Iterate through final results and print results of each candidate. Also find the winner based on the maximum number of votes.
	int maxVotes = 0;
	int maxIndex = 0;
	vector<Candidate*> candidates = elec -> get_candidates();
	for(int i = 0; i < finalVotes.size(); i++)
	{
		cout << candidates[i] -> name_ << ": " << finalVotes[i] << endl;
		// Determine if more votes than maximum
		if (finalVotes[i] > maxVotes)
		{
			maxVotes = finalVotes[i];
			maxIndex = i;
		}
	}
	// Print out total winner
	cout << endl << "Congratulations, " << candidates[maxIndex] -> name_ << ", you've won!" << endl;
	cout << "-----------------" << endl;
}