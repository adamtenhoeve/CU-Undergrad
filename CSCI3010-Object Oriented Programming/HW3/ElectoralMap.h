#ifndef _ELECTORALMAP_H_
#define _ELECTORALMAP_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

// Names: Adam Ten Hoeve

// Enum class of all parties in the race
enum class PartyType {Lions, Tigers, Bears, None, Count};

// Get vector of all parties
vector<PartyType> GetParties();

// Returns the string version of the party
string StringifyPartyType(PartyType);

struct Candidate
{
	string name_;
	PartyType party_;
	int id_;

	// Constructor based on passed in name from user, party selected, and id based on candidate created.
	Candidate(string name, PartyType p, int id) 
	{
		name_ = name;
		party_ = p;
		id_ = id;
	}

	friend std::ostream& operator<<(std::ostream &os, const Candidate);
};

class District 
{
public:
	// Constructor, given the district id
	District(int);
	// 
	int get_area() {return area_;};
	// Returns district id
	int get_id() {return district_id_;};
	// Returns the vector of constituents of each party.
	vector<int> get_constituents() {return constituents_;};
	// Convert a constituent from party None to input party
	void ConvertNone(int candParty);
	// Convert a constituent from the majortiy party to the input party
	void ConvertOther(int candParty);
private:
	// Each district is of a certain area.
	int area_;
	int district_id_;
	vector<int> constituents_;

	friend std::ostream& operator<<(std::ostream &os, const District &d);
};


class ElectoralMap {
public:

	// The number of districts created in the map. Can be changed without breaking things.
	static const int num_districts_ = 4;

	static ElectoralMap& GetInstance() 
	{
		// Gets instantiated the first time
		static ElectoralMap instance; // Guaranteed to be destroyed
		return instance;
	}

	// Returns a pointer to the district at that id
	District* get_district(int id) {return districts_.at(id);};

	// Delete the methods we don't want
	ElectoralMap(ElectoralMap const&) = delete;
	void operator=(ElectoralMap const&) = delete;

private:
	// Private constructor
	ElectoralMap();
	// ID for assigning to each district in map
	int district_id;
	// Map containing id and district at that id
	std::map<int, District *> districts_;

	friend std::ostream& operator<<(std::ostream &os, const ElectoralMap &e);

};


# endif