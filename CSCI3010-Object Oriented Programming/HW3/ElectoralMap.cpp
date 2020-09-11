#include <iostream>
#include <map>
#include <stdlib.h>
#include "ElectoralMap.h"

// Name: Adam Ten Hoeve

using namespace std;

vector<PartyType> GetParties()
{
	vector<PartyType> PartyVec;
	// Get the number of parties from "Count" element in the enum class. This is the number of parties in the election.
	int num_parties = (int)PartyType::Count;
	// For each party, add it to the PartyType vector so it can be returned. Does not add the "Count" enum.
	for(int i = 0; i < num_parties; i++)
	{
		PartyVec.push_back(PartyType(i));
	}
	return PartyVec;
}

// Returns a stringified version of all the parties;
string StringifyPartyType(PartyType p)
{
	switch(p)
	{
		case PartyType::None:
			return("None");

		case PartyType::Lions:
			return("Lions");

		case PartyType::Tigers:
			return("Tigers");

		case PartyType::Bears:
			return("Bears");

		default:
			return("Error");
	}
}

// District constructor. Sets the size, district id and number of constituents for each party.
District::District(int districtID)
{
	// Set a random area between 5 and 29
	area_ = rand() % 25 + 5;

	// Set the district ID based on value passed from ElectoralMap
	district_id_ = districtID;

	// Figure out the number of parties
	int num_parties_ = (int)PartyType::Count;
	// For each party, assign a random number of constituents between 0 and 9
	for(int i = 0; i < num_parties_; i++)
	{
		constituents_.push_back(rand() % 10); // random number between 0 and 9
	}
}

// Convert a constituent from Party::None to passed in party.
void District::ConvertNone(int candParty)
{
	// Check to make sure there is at least one constituent in Party::None.
	if(constituents_.back() > 0)
	{
		// Remove 1 from the none party and 
		constituents_.back() -= 1;
		constituents_[candParty] += 1;
		// Print to user that someone was converted.
		cout << "Congrats, you have converted someone from none to " << StringifyPartyType(PartyType(candParty)) << "!" << endl;
	}
	// If no constituents in Party::None, then print that to the user
	else
	{
		cout << "Sorry, no on in none to covert!" << endl;
	}
}

// Convert a constituent from the majority party to the passed in party
void District::ConvertOther(int candParty)
{
	// Initialize variables for the maxConstituents and the index
	int maxConstituents = 0;
	int maxIndex = 0;
	// Find the party (that is not the candidate's party) with the majority of constituents. Does not condiser party::None.
	for(int i = 0; i < constituents_.size()-1; i++)
	{
		// Do not consider the candidate's party.
		if (i == candParty) {continue;}
		// Finds the party with the maximum constituents.
		if (constituents_[i] > maxConstituents)
		{
			maxConstituents = constituents_[i];
			maxIndex = i;
		}
	}

	// Make sure that there is at least one person to be converted
	if (maxConstituents > 0)
	{
		// Remove one constituent from the max party and add one to the candidate's party
		constituents_[maxIndex] -= 1;
		constituents_[candParty] += 1;
		// Print to the user what party was converted from.
		cout << "Congrats, you have converted someone from " << StringifyPartyType(PartyType(maxIndex));
		cout << " to " << StringifyPartyType(PartyType(candParty)) << "!" << endl;
	}
	// Otherwise no one to convert
	else
	{
		cout << "Sorry, no one in none to convert!" << endl;
	}
}


/////////////////////////////////////////  Electoralmap Class /////////////////////////////////////////////////////////////////////

ElectoralMap::ElectoralMap()
{
	district_id = 0;
	// Initialize a number of districts within the map equal to the number specified
	for(int i = 0; i < num_districts_; i++)
	{
		districts_[i] = new District(i+1);
		district_id += 1;
	}
}

std::ostream& operator<<(std::ostream &os, const Candidate &c)
{
	os << c.id_ << ": " << c.name_ << " [Party: " << StringifyPartyType(c.party_) << "]";
	return os;
}

std::ostream& operator<<(std::ostream &os, const District &c) {
	// Return the area of the district in the ostream.
	os << "Area: " << c.area_;
    return os;
}

std::ostream& operator<<(std::ostream &os, const ElectoralMap &c) {
	// For each district in the map, find the area of that district and add it to the ostream.
    for(int i = 0; i < c.num_districts_; i++)
    {
    	os << c.districts_.at(i) -> get_area() << endl;
    }
    return os;
}
