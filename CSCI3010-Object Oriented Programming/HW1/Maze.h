#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include "Player.h"
#include <iostream>

using namespace std;

// you may change this enum as you need
enum class SquareType { Wall, Exit, Empty, Human, Enemy, Treasure1, Treasure2, Treasure3 };

std::string SquareTypeStringify(SquareType sq);

class Board {
public:
	Board();
	
	// already implemented in line
	int get_rows() const {return rows_; }  // you should be able to change the size of your 
	int get_cols() const {return cols_; }  // board by changing these numbers and the numbers in the arr_ field

//	// TODO: you MUST implement the following functions
	SquareType get_square_value(Position pos) const; 
//
//	// set the value of a square to the given SquareType
	void SetSquareValue(Position pos, SquareType value);
//
//	// get the possible Positions that a Player could move to
//	// (not off the board or into a wall)
	std::vector<Position> GetMoves(Player *p);
//
//	// Move a player to a new position on the board. Return
//	// true if they moved successfully, false otherwise.
	bool MovePlayer(Player *p, Position pos);
//
//	// Get the square type of the exit square
	SquareType GetExitOccupant();

	void PrintBoard();
//
//	// You probably want to implement this
	// friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
	SquareType arr_[4][4];
	
	int rows_; // might be convenient but not necessary
	int cols_;
	
	// you may add more fields, as needed
};  // class Board

class Maze {
public:
	Maze(); // constructor

	// initialize a new game, given one human player and 
	// a number of enemies to generate
	void NewGame(Player *human, const int enemies);

	// have the given Player take their turn
	void TakeTurn(Player *p);

	// Get the next player in turn order
	Player * GetNextPlayer();

	// return true iff the human made it to the exit 
	// or the enemies ate all the humans
	bool IsGameOver();

	// My own functions
	// Converts the returned positions from Board's GetMoves() to a vector of strings to the corresponding directions.
	vector<string> MovesToString(Player *p, vector<Position>);
	// Converts the input string of the user to a location to move to.
	Position StringToMove(Player *p, string);
	// Function for placing the enemies when the game is created. Always places them in space not occupied by walls, treasure,
	// the player, other enemies, or the exit.
	void SetPosition(Player*);

	// You probably want to implement these functions as well
	// string info about the game's conditions after it is over
	void GenerateReport();
	// friend std::ostream& operator<<(std::ostream& os, const Maze &m);

private:
	Board *board_; 
	std::vector<Player *> players_;
	int turn_count_;
	int player_index_;

	// you may add more fields, as needed

};  // class Maze

#endif  // _MAZE_H_
