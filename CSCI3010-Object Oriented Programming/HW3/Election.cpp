#include <iostream>
#include <vector>
#include <math.h>
#include "Election.h"
#include "ElectoralMap.h"

using namespace std;

// Name: Adam Ten Hoeve

Election::Election()
{
	// Create an instance of the static class ElectoralMap
	ElectoralMap &temp = ElectoralMap::GetInstance();
	map = &temp;
}

// Adds a candidate to the vector of candidates that are running in the election.
void Election::AddCandidate(string name, PartyType party)
{
	Candidate *c = new Candidate(name, party, candidates_.size() + 1);
	candidates_.push_back(c);
}

// Returns a vector of the districts in the ElectoralMap
vector<District*> Election::get_districts()
{
	// Create a vector of districts to be returned.
	vector<District*> vec;
	// Iterate over all the districts in the ElectoralMap.
	for(int i = 0; i < map -> num_districts_; i++)
	{
		vec.push_back(map -> get_district(i));
	}

	return(vec);
}

// Calculates the result of a candidate campaigning in a district
void Election::CandidateIsCampaigning(int candID, int distID)
{
	// Get the district from the district ID
	District *dist = map -> get_district(distID);

	// Get a vector of all the parties
	vector<PartyType> parties = GetParties();

	// Determine which party the candidate is affiliated with. 
	// Will be used as the index to get the constituents of the party for the district.
	int candParty = (int)candidates_[candID] -> party_;
	// Get the number of constituents of the candidate's party.
	int partyConstituents = dist -> get_constituents()[candParty];

	// Get the number of constituents from the other parties (not including Party::None), which is why we do size - 1.
	int otherPartyConstituents = 0;
	for (int i = 0; i < parties.size() - 1; i++)
	{
		// Check to make sure not including constituents of the candidate's party. Don't include those.
		if (i == candParty) {continue;}
		// Sum up all other constituents.
		otherPartyConstituents += dist -> get_constituents()[i];
	}

	float success = 0;
	float extraSuccess = 0;
	// Check to make sure otherPartyConstituents does not equal zero. Otherwise things will break.
	if (otherPartyConstituents != 0)
	{
		// Calculate success prob from equation
		success = ((float)(partyConstituents + 1)*2 / otherPartyConstituents) * ((float)(partyConstituents + 1)*2 / dist -> get_area());
		// Calculate extra success. extraSuccess = 0.1 * success
		extraSuccess = 0.1 * success;
		// Output chances to convert to the user
		cout << "Chances to convert: " << success << endl;
		cout << "Chances to convert from another party: " << extraSuccess << endl;
	}
	else
	{
		// If otherPartyConstituents are 0, then can not convert any more people from other parties. Infinite chance of converting.
		success = 1000;
		extraSuccess = 100;
		cout << "Chances to convert: inf" << endl;
		cout << "Chances to convert from another party: inf" << endl; 
	}

	// Generate a random number to determine if will convert based on success probabilities.
	float RV = (float)rand() / RAND_MAX;
	// Determine the outcome based on if the random variable is less than the calculated probs.
	bool willConvertNone = RV <= success;
	bool willConvertOther = RV <= extraSuccess;

	// 4 situtions that can occur based on above 2 booleans.
	// Scenario 1: Will convert someone from party None to candidate's party
	// The function ConvertNone takes care of the case that there are no constituents in Party::None.
	if(willConvertNone)
	{
		dist -> ConvertNone(candParty);
	}
	// Scenario 2: Will convert someone from party::None and majority party.
	// Scenario 3: Will convert someone from majority party. ConvertNone handles the case that there are no consitituents in party::None.
	if(willConvertOther)
	{
		dist -> ConvertOther(candParty);
	}
	// Scenario 4: Campaigning causes no one to convert. Do not need to code anything for this.
}

// Calculate and tally the votes for each candidate.
vector<int> Election::Votes()
{
	// Want to calculate results per Candidate.
	// Create new vector of ints for each candidate with 0 votes each.
	vector<int> totalVotes(candidates_.size(), 0);
	// Create a vector of votes per party, not include Party::None. 
	// Constituents will vote per party, then party will randomly distribute votes to candidates.
	vector<int> votesPerParty(GetParties().size()-1, 0);
	// Create a vector of booleans for each party for whether there is a candidate running for that party.
	// Do not want to add votes to parties that do not have candidates running.
	vector<bool> partyHasCandidates(GetParties().size()-1, false);

	// Determine which parties have candidates running. Loop over all candidates and set the partyHadCandidates to true
	// for their party.
	for (int i = 0; i < candidates_.size(); i++)
	{
		partyHasCandidates[(int)(candidates_[i] -> party_)] = true;
	}

	// Now want to calculate votes per district.
	// Get all districts in the ElectoralMap
	vector <District*> dists = get_districts();
	// Iterate over all the districts
	for(int i = 0; i < dists.size(); i++)
	{
		// Print to user what district is being calculated
		cout << "District: " << dists[i] -> get_id() << endl;
		// Get the vector of constituents for the district.
		vector<int> constituents = dists[i] -> get_constituents();
		// Find the party with the majority number of constituents, not counting Party::None.
		int maxConstituents = 0;
		int maxIndex = 0;
		for (int i = 0; i < constituents.size() - 1; i++)
		{
			// Reassign as the max number of constituents
			if(constituents[i] > maxConstituents)
			{
				maxConstituents = constituents[i];
				maxIndex = i;
			}
		}

		// Loop through vector of constituents to calculate the votes per constituent.
		for(int j = 0; j < constituents.size(); j++)
		{
			// constituents[j] is the number of constituents for party j
			// Each constituent votes semi-randomly, so need to repeat loop for each constituent.
			for (int k = 0; k < constituents[j]; k++)
			{
				// Generate a random number to decide how they vote
				float RV = (float) rand() / RAND_MAX;

				// Check if party j is Party::None, which is the last in the vector or the party doesn't have a candidate running.
				if((j == constituents.size() - 1) || (!partyHasCandidates[j]))
				{
					// Vote for the majority party 70% of the time
					if ((RV <= 0.7) && (partyHasCandidates[maxIndex]))
					{
						votesPerParty[maxIndex] += 1;
					}
					// Otherwise randomly votes for a party that is not the majority.
					else
					{
						// Generate a random int for the number of parties. 
						// May generate the majority party, so repeat until it doesn't.
						// Will have a limit of iterations because may not have any candidates of these parties.
						for(int count = 0; count < 20; count++)
						{
							int party = rand() % votesPerParty.size();
							if ((party != maxIndex) && (partyHasCandidates[party]))
							{
								votesPerParty[party] += 1;
								break;
							}
						}
					}
				}
				else
				// The constituent is not part of Party::None and a candidate of their party is running.
				{
					// Votes for thier party 90% of the time.
					if ((RV <= 0.9) && (partyHasCandidates[j]))
					{
						votesPerParty[j] += 1;
					}
					else
					{
						// May run forever if the only candidate is of majority party so set a limit to how long it will run.
						for(int count = 0; count < 20; count++)
						{
							// Votes for a party that is not their own party (j). Generates random int to decide this.
							// May generate own party, so repeat until it doesn't.
							int party = rand() % votesPerParty.size();
							if ((party != j) && (partyHasCandidates[party]))
							{
								votesPerParty[party] += 1;
								break;
							}
						}
					}
				}
			}
		}

		// Vector to store votes for each candidate for the district
		vector<int> candVotesPerDist(candidates_.size(), 0);

		// Now have a vector of how many votes the party got in dist [i]. Need to randomly assign those votes to candidates of that party.
		for(int j = 0; j < votesPerParty.size(); j++)
		{
			// Assign votes for candiates of the party
			while(votesPerParty[j] != 0)
			{
				// Generate a random variable for which candidate to select
				int RV = rand() % candidates_.size();

				// Determine if the random candidate is of party [j]. If not, restart loop.
				if ((int)(candidates_[RV] -> party_ ) != j) 
				{
					continue;
				}

				// Add a vote to that candidate and remove a vote from the party.
				candVotesPerDist[RV] += 1;
				votesPerParty[j] -= 1;
			}
		}
		// Assigned all party votes to candidates of that party.
		// Add votes to total and print out how many votes for each candidate.
		for(int j = 0; j < candVotesPerDist.size(); j++)
		{
			cout << candidates_[j] -> name_ << ": " << candVotesPerDist[j] << endl;
			// Add votes for district to total votes
			totalVotes[j] += candVotesPerDist[j];
		}
	}

	return totalVotes;
}

vector<int> RepresentativeElection::Votes()
{
	// Vector of final votes per candidate.
	vector<int> finalVotes(get_candidates().size(), 0);

	// Get the ElectoralMap from the parent class
	ElectoralMap* map = get_map();

	// Total number of votes = 5 * number of districts
	int totalVotes = 5 * map -> num_districts_;

	// Find the number of votes per district based on the size of each district. Initialize the total number of constituents,
	// a vector of total constituents per district, and a vector of all the districts.
	int totalConstituents = 0;
	vector<District*> districts = get_districts();
	vector<int> numConstituents(districts.size(), 0);

	// Loop over all the distrcits to find the total number of constituents.
	for(int i = 0; i < districts.size(); i++)
	{
		// Get the vector of constituents
		vector<int> constituents = districts[i] -> get_constituents();
		// Sum all constituents in that vector to get the total number of that district, and do that for all districts to
		// get total number of constituents.
		for(int j = 0; j < constituents.size(); j++)
		{
			numConstituents[i] += constituents[j];
			totalConstituents += constituents[j];
		}
	}

	// Now have vector of constituents per district, can calculate the number of votes each district has.
	vector<int> numVotes(numConstituents.size(), 0);
	for(int i = 0; i < numConstituents.size(); i++)
	{
		numVotes[i] = floor(((float)(numConstituents[i]) / totalConstituents) * totalVotes);
	}

	// Now want to calculate who each person votes for, the same way we did in original election.

	// Create a vector of votes per party, not include Party::None. 
	// Constituents will vote per party, then party will randomly distribute votes to candidates.
	vector<int> votesPerParty(GetParties().size()-1, 0);
	// Create a vector of booleans for each party for whether there is a candidate running for that party.
	// Do not want to add votes to parties that do not have candidates running.
	vector<bool> partyHasCandidates(GetParties().size()-1, false);

	// Determine which parties have candidates running. Loop over all candidates and set the partyHadCandidates to true
	// for their party.
	for (int i = 0; i < get_candidates().size(); i++)
	{
		partyHasCandidates[(int)(get_candidates()[i] -> party_)] = true;
	}

	// Have vector of votes per district, want to repeat the assignment from original function.
	// Then whomever has the most votes at the end of the district election gets all votes for that district.
	for (int i = 0; i < districts.size(); i++)
	{
		// Print to user what district is being calculated
		cout << "District: " << districts[i] -> get_id() << endl;
		// Get the vector of constituents for the district.
		vector<int> constituents = districts[i] -> get_constituents();
		// Find the party with the majority number of constituents, not counting Party::None.
		int maxConstituents = 0;
		int maxIndex = 0;
		for (int i = 0; i < constituents.size() - 1; i++)
		{
			// Reassign as the max number of constituents
			if(constituents[i] > maxConstituents)
			{
				maxConstituents = constituents[i];
				maxIndex = i;
			}
		}

		// Loop through vector of constituents to calculate the votes per constituent.
		for(int j = 0; j < constituents.size(); j++)
		{
			// constituents[j] is the number of constituents for party j
			// Each constituent votes semi-randomly, so need to repeat loop for each constituent.
			for (int k = 0; k < constituents[j]; k++)
			{
				// Generate a random number to decide how they vote
				float RV = (float) rand() / RAND_MAX;

				// Check if party j is Party::None, which is the last in the vector or the party doesn't have a candidate running.
				if((j == constituents.size() - 1) || (!partyHasCandidates[j]))
				{
					// Vote for the majority party 70% of the time
					if ((RV <= 0.7) && (partyHasCandidates[maxIndex]))
					{
						votesPerParty[maxIndex] += 1;
					}
					// Otherwise randomly votes for a party that is not the majority.
					else
					{
						// Generate a random int for the number of parties. 
						// May generate the majority party, so repeat until it doesn't.
						// Will have a limit of iterations because may not have any candidates of these parties.
						for(int count = 0; count < 20; count++)
						{
							int party = rand() % votesPerParty.size();
							if ((party != maxIndex) && (partyHasCandidates[party]))
							{
								votesPerParty[party] += 1;
								break;
							}
						}
					}
				}
				else
				// The constituent is not part of Party::None and a candidate of their party is running.
				{
					// Votes for thier party 90% of the time.
					if ((RV <= 0.9) && (partyHasCandidates[j]))
					{
						votesPerParty[j] += 1;
					}
					else
					{
						// May run forever if the only candidate is of majority party so set a limit to how long it will run.
						for(int count = 0; count < 20; count++)
						{
							// Votes for a party that is not their own party (j). Generates random int to decide this.
							// May generate own party, so repeat until it doesn't.
							int party = rand() % votesPerParty.size();
							if ((party != j) && (partyHasCandidates[party]))
							{
								votesPerParty[party] += 1;
								break;
							}
						}
					}
				}
			}
		}

		// Vector to store votes for each candidate for the district
		vector<int> candVotesPerDist(get_candidates().size(), 0);

		// Now have a vector of how many votes the party got in dist [i]. Need to randomly assign those votes to candidates of that party.
		for(int j = 0; j < votesPerParty.size(); j++)
		{
			// Assign votes for candiates of the party
			while(votesPerParty[j] != 0)
			{
				// Generate a random variable for which candidate to select
				int RV = rand() % get_candidates().size();

				// Determine if the random candidate is of party [j]. If not, restart loop.
				if ((int)(get_candidates()[RV] -> party_ ) != j) 
				{
					continue;
				}

				// Add a vote to that candidate and remove a vote from the party.
				candVotesPerDist[RV] += 1;
				votesPerParty[j] -= 1;
			}
		}
		
		// Have vector for number of votes for each candidate. Want to find maximum votes and assign all votes for the 
		// district to that candidate.
		int maxVotes = 0;
		int maxIdx = 0;
		// Loop over all candidates and find the one with the most votes.
		for(int j = 0; j < candVotesPerDist.size(); j++)
		{
			if(candVotesPerDist[j] > maxVotes)
			{
				maxVotes = candVotesPerDist[j];
				maxIdx = j;
			}
		}

		// Assign all the votes for the dist to that candidate.
		finalVotes[maxIdx] += numVotes[i];
		cout << get_candidates()[maxIdx] -> name_ << ": " << numVotes[i] << endl;
	}
	return finalVotes;
}