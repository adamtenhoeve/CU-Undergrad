// Adam Ten Hoeve
// CSCI3010
// main.cpp of HW1

#include <iostream>
#include "Player.h"
#include "Maze.h"

using namespace std;

// Welcome to my game. For this version, I used capital characters and numbers to represent the items in the game.
// The human character is H
// The enemies are E
// The exit is X
// The walls are W
// Empty space is O
// 1,2,3 are treasures representing 100, 200, and 300 points respectively.
// Enjoy!

int main()
{
	// Iniitalizes a human player. It uses my name to start out with.
	Player *p1 = new Player("Adam", true);
	// Initialize the maze that runs the game.
	Maze *maze = new Maze;
	// Start up a new game. Initially set with 2 enemies but that can be changed. Enemies are randomly placed.
	maze -> NewGame(p1, 2);
	// Game continues to play until the human reaches the exit or runs into an enemy.
	while(!maze -> IsGameOver())
	{
		// Iterates through the human and enemy players to be controlled, in order.
		Player *currentPlayer = maze -> GetNextPlayer();
		// In this function, you decide where the player moves within the possible directions.
		// This function also prints out the board
		maze -> TakeTurn(currentPlayer);
	}
	// Once the game is over, prints out the scores of the human and the enemies.
	maze -> GenerateReport();

	// Make sure to free the allocated memory.
	delete maze;
	delete p1;
	// Thanks for playing! To play again, please rerun the executable.
}