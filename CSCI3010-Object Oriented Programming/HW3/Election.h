#ifndef _ELECTION_H_
#define _ELECTION_H_

#include <iostream>
#include <vector>
#include "ElectoralMap.h"

using namespace std;

// Create election class. Contains Candidates, assigns candidates to districts, and tallies votes at the end.
class Election
{
public:
	Election();

	// Returns a vector of the candidates running in the election.
	vector<Candidate*> get_candidates() {return candidates_;};

	// Returns a vector of the districts in the ElectoralMap.
	vector<District*> get_districts();

	ElectoralMap* get_map() {return map;};

	// Add a candidate to the vector of candidates.
	void AddCandidate(string, PartyType);

	// Function to calculate the results of a candidate campaigning
	void CandidateIsCampaigning(int candID, int distID);

	// Calls for the final votes to be calculated.
	virtual vector<int> Votes(); 

private:
	ElectoralMap *map;
	vector<Candidate*> candidates_;
};


// Create a representative election that is a subclass of election
class RepresentativeElection : public Election
{
public:
	RepresentativeElection(): Election() {};
	// Calculate the number of votes given the Representative voting style.
	vector<int> Votes();
private:
	
};

# endif