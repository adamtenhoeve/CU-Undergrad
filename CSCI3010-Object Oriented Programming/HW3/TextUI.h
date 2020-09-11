#ifndef _TEXTUI_H_
#define _TEXTUI_H_

#include <iostream>
#include "Election.h"
#include "ElectoralMap.h"
using namespace std;

// Name: Adam Ten Hoeve

class TextUI
{
public:
	// Wrapper function to add all the candidates to the election.
	void AddCandidates(vector<PartyType>, Election* elec);
	// Prints all the districts to the user, including the district name, id, and the number of constituents.
	void PrintDistricts(Election *elec);
	// Prints all the cadidates to the user, including their id and party affiliation
	void PrintCandidates(Election *elec);
	// User selects a candidate and a district to campaign in. They are prompted until they exit.
	void Campaigning(Election *elec);
	// Constituents vote for candidates based on party lines.
	void Voting(Election *elec);
};



#endif