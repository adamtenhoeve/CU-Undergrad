// Adam Ten Hoeve
// CSCI 3010 HW 1
// Methods for the board and maze classes.

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "Maze.h"
#include "Player.h"

using namespace std;

// Board Methods

// Return a string version of the enum passed in.
// { Wall, Exit, Empty, Human, Enemy, Treasure }
string SquareTypeStringify(SquareType sq)
{
	// int n = static_cast<int>(sq);
	// Case match on the enum and return associated string.
	switch(sq)
	{
		case SquareType::Wall:
			return ("W");

		case SquareType::Exit:
			return ("X");

		case SquareType::Empty:
			return ("O");

		case SquareType::Human:
			return ("H");

		case SquareType::Enemy:
			return ("E");

		case SquareType::Treasure1:
			return ("1");

		case SquareType::Treasure2:
			return ("2");

		case SquareType::Treasure3:
			return ("3");

		default:
			return("Error.");
	}
}

// Board Constructor. Sets the size of the board, inital locations of the player, exit and enemies, then randomly generates
// walls and treasures.
Board::Board()
{
	// Set the seed for the random numbers
	srand(time(NULL));

	// Set the size of the board
	rows_ = 4;
	cols_ = 4;

	// Set the starting location of the player in the top left corner
	arr_[0][0] = SquareType::Human;
	// Set the location of the exit in the bottom right corner
	arr_[rows_-1][cols_-1] = SquareType::Exit;

	// Iterate through the entire board to randomly place walls and treasures
	for(int i = 0; i < rows_; i++)
	{
		for(int j = 0; j < cols_; j++)
		{
			// Do not overwrite any location that has a player, exit, or enemies
			if(arr_[i][j] != SquareType::Human && arr_[i][j] != SquareType::Exit && arr_[i][j] != SquareType::Enemy)
			{
				// 20% chance of making a wall. Generate random int between 0 and 4.
				// If random number is 0, set as wall.
				int rNum = rand() % 5;
				if(rNum == 0)
				{
					arr_[i][j] = SquareType::Wall;
					continue;
				}
				// If was not a wall, then 10% chance to be a treasure. 3 different treasures, each with 20% probability
				// Generate random int between 0 and 4. If is 0, then is treasure1. If 1, then treasure2. If 2, then Treasure3.
				rNum = rand() % 5;
				if(rNum == 0)
				{
					arr_[i][j] = SquareType::Treasure1;
					continue;
				}
				else if(rNum == 1)
				{
					arr_[i][j] = SquareType::Treasure2;
					continue;
				}
				else if(rNum == 2)
				{
					arr_[i][j] = SquareType::Treasure3;
					continue;
				}
				// If neither a treasure or a wall, set value of square to empty
				arr_[i][j] = SquareType::Empty;
			}
		}
	}
}

// Returns the item that is in a board location
SquareType Board::get_square_value(Position pos) const
{
	return(arr_[pos.row][pos.col]);
}

// Sets the value of a square to the designated value
void Board::SetSquareValue(Position pos, SquareType value)
{
	arr_[pos.row][pos.col] = value;
}

// Determines which directions a player is allowed to go.
// Based on if the player is at the edge of the board or next to a wall.
vector<Position> Board::GetMoves(Player *p)
{
	vector<Position> pos;
	// Get the row and column of the player so don't have to use the methods each time
	int row = p -> get_position().row;
	int col = p -> get_position().col;
	//Check the 4 directions to see which ones are valid paths.
	// Check the upward direction. If the up index is 0, then can not move upward b/c at top of the grid.
	if(row != 0)
	{
		// Check if there is a wall above of the player. Check here so we don't go out of bounds of the grid.
		if(arr_[row-1][col] != SquareType::Wall)
		{
			Position temp;
			temp.row = row - 1;
			temp.col = col;
			pos.push_back(temp);
		}
	}

	// Repeat process for down, left, and right
	// Checking if can move down
	if(row != rows_-1)
	{
		if(arr_[row+1][col] != SquareType::Wall)
		{
			Position temp;
			temp.row = row + 1;
			temp.col = col;
			pos.push_back(temp);
		}
	}

	// Check if can move left
	if(col != 0)
	{
		if(arr_[row][col-1] != SquareType::Wall)
		{
			Position temp;
			temp.row = row;
			temp.col = col - 1;
			pos.push_back(temp);
		}
	}

	// Check if can move right
	if(col != cols_-1)
	{
		if(arr_[row][col+1] != SquareType::Wall)
		{
			Position temp;
			temp.row = row;
			temp.col = col + 1;
			pos.push_back(temp);
		}
	}
	// Return the vector of possible paths
	return(pos);
}

bool Board::MovePlayer(Player *p, Position pos)
{
	// Get a vector of the possible moves from the GetMoves() function.
	vector<Position> moves;
	moves = GetMoves(p);

	// cout << "Inside MovePlayer: " << pos.row << " " << pos.col << endl;
	// Determine if desired move pos is possible (within the moves vector). Loop through moves vector and compare to desired position.
	for(vector<Position>::size_type i = 0; i < moves.size(); i++)
	{
		if(pos == moves[i])
		{
			// If it is, move the player there and return true.
			// Get the current position of the player
			Position currentPos;
			currentPos = p -> get_position();

			// Special conditions for moving into certain SquareType
			// If a human is moving into an enemy, destroy the player
			if(p -> is_human() && arr_[pos.row][pos.col] == SquareType::Enemy)
			{
				// Set the human location as empty and don't change the enemy to appear as if the player was deleted.
				arr_[currentPos.row][currentPos.col] = SquareType::Empty;
				return(true);
			}

			// If an enemy moves into a player, want to destroy the player
			if((p -> is_human() == false) && (arr_[pos.row][pos.col] == SquareType::Human))
			{
				arr_[currentPos.row][currentPos.col] = SquareType::Empty;
				arr_[pos.row][pos.col] = SquareType::Enemy;
			}

			// If an enemy is moving into a space with another enemy, nothing happens. They bounce.
			if((p -> is_human() == false) && (arr_[pos.row][pos.col] == SquareType::Enemy))
			{
				// Don't move anyone. Can just return true.
				return(true);
			}

			// If the player is moving into a space with treasure, add points to that player.
			// 100 points for Treasure1. 200 for Treasure2. 300 for Treasure3.
			if(arr_[pos.row][pos.col] == SquareType::Treasure1)
			{
				p -> ChangePoints(100);
			}
			else if(arr_[pos.row][pos.col] == SquareType::Treasure2)
			{
				p -> ChangePoints(200);
			}
			else if(arr_[pos.row][pos.col] == SquareType::Treasure3)
			{
				p -> ChangePoints(300);
			}

			// Set the current position of the player to an empty cell.
			arr_[currentPos.row][currentPos.col] = SquareType::Empty;
			p -> SetPosition(pos);
			// Then set the new position on board to an H or E, depending on isHuman of player.
			if(p -> is_human())
			{
				arr_[pos.row][pos.col] = SquareType::Human;
			}
			else
			{
				arr_[pos.row][pos.col] = SquareType::Enemy;
			}
			return(true);
		}
	}
	// If the desired position is not a possible position, return false
	return(false);
}

// Returns the SqareType at the bottom right corner of the grid.
SquareType Board::GetExitOccupant()
{
	return(arr_[rows_-1][cols_-1]);
}

// Function to print the board.
void Board::PrintBoard()
{
	// Iterate over entire grid
	for(int i = 0; i < rows_; i++)
	{
		for(int j = 0; j < cols_; j++)
		{
			// Print the string associated with the SquareType in each square
			cout << SquareTypeStringify(arr_[i][j]) << " ";
		}
		cout << endl;
	}
	cout << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maze Methods

// Constructor. Creates a board object, a vector of players, and a turn_count_ to determine what player is active.
Maze::Maze()
{
	// Set the turn counter to 0
	turn_count_ = -1;
	// Create an empty vector of players
	vector<Player *> temp;
	players_ = temp;
	// Set index of players to -1. Will become the first player when game is started.
	player_index_ = -1;
	// Create a pointer to a board
	board_ = new Board();
}

// Function to add players to the players_ vector and populate the board to begin a new game.
void Maze::NewGame(Player *human, const int enemies)
{
	// Add the player to the beginning of the vector of players. The human will always start at (0,0) because it was initialized there 
	// in the Player constructor and has not been changed.
	players_.push_back(human);

	// For each enemy, create a player and add it to the players_ vector. Each enemy has a unique name based on the number
	// of enemies created.
	for(int i = 0; i < enemies; i++)
	{
		// Create a new enemy
		Player *enemy = new Player("Enemy" + to_string(i), false);
		// Set them at a random location. See line 495.
		SetPosition(enemy);
		// Add the enemy to the vector of players.
		players_.push_back(enemy);
	}
	// Print the Starting Board
	board_ -> PrintBoard();
}

// Function for a player to move in a lateral direction. Determines possible moves from position on board and lets player move
// in one of those directions.
void Maze::TakeTurn(Player *p)
{
	// Get the possible moves of the player. See line 124.
	vector<Position> moves = board_ -> GetMoves(p);
	// Determine if can move UP, DOWN, LEFT, RIGHT, from possible moves. See line 426.
	vector<string> movesStr = MovesToString(p, moves);
	// Print the possible moves, as strings, to the player
	cout << p -> get_name() << " can go: ";
	for(unsigned i = 0; i < movesStr.size(); i++)
	{
		cout << movesStr[i] << " ";
	}
	cout << endl;

	// Get the players input of direction to move. Only accepts input up, down, left, right (case insensitive).
	string choice;
	cout << "Please enter your choice: ";
	cin >> choice;
	cout << endl;
	// Check to make sure the input is valid.
	bool valid = false;
	while(valid == false)
	{
		// Make the input case insensitive. Converts everything to lowercase.
		for(unsigned i = 0; i < choice.length(); i++)
		{
			choice[i] = tolower(choice[i]);
		}
		//Check if the input string is one of the possible direction strings.
		for(unsigned i = 0; i < movesStr.size(); i++)
		{
			if(choice == movesStr[i])
			{
				valid = true;
			}
		}
		// If it is not valid, prompt the use to input another string.
		if(valid == false)
		{
			cout << "Invalid input. Please enter 'up', 'down', 'left' or 'right'" << endl;
			cin >> choice;
		}
	}
	// Converts the input string to a direction. See line 462.
	Position nextPos;
	nextPos = StringToMove(p, choice);

	// Move the player in the desired direction. See line 184.
	bool didMove;
	didMove = board_ -> MovePlayer(p, nextPos);

	board_ -> PrintBoard();
}

// Funtion that returns the next player in the players vector. Once all players have gone, loops back to first player.
Player* Maze::GetNextPlayer()
{
	// Use the turn_count to determine which player is going.
	turn_count_ += 1;
	// Loops back to 0 if goes over the length of the vector.
	if(turn_count_ == players_.size())
	{
		turn_count_ = 0;
	}
	return(players_[turn_count_]);
}

// Determine if the game is over based on whether the human has made it to the exit or died.
bool Maze::IsGameOver()
{
	// Game is over if the player is deleted (eaten by an enemy).
	bool isPlayerAlive = false;
	// Iterate over entire board to see if Human SquareType is still there
	for(int i = 0; i < board_ -> get_rows(); i++)
	{
		for(int j = 0; j < board_ -> get_cols(); j++)
		{
			Position pos;
			pos.row = i;
			pos.col = j;
			// If any square contains the human, then set isPlayerAlive to true and do not end the game.
			if(board_ -> get_square_value(pos) == SquareType::Human)
			{
				isPlayerAlive = true;
			}
		}
	}
	if(!isPlayerAlive)
	{
		return(true);
	}

	// Game is also over if the player reaches the exit.
	if(board_ -> GetExitOccupant() == SquareType::Human)
	{
		// Add one point to the player's score for reaching the exit. Human player is always the first player in the players vector.
		players_[0] -> ChangePoints(1);
		return(true);
	}
	// Otherwise game is still going.
	return(false);
}

// Coverts the position coordinates to strings of UP, DOWN, LEFT, RIGHT
vector<string> Maze::MovesToString(Player *p, vector<Position> moves)
{
	// Initialize a vector for the string directions.
	vector<string> str; 
	Position playerPos = p -> get_position();
	int row = 0;
	int col = 0;
	// Takes the vector of coordinates and pushes stings to a new vector for the matching directions.
	for(unsigned i = 0; i < moves.size(); i++)
	{
		// Determine relative diretions in terms of coordinates.
		row = moves[i].row - playerPos.row;
		col = moves[i].col - playerPos.col;
		// Convert relative coordinates to strings.
		if(row == 1)
		{
			str.push_back("down");
		}
		else if (row == -1)
		{
			str.push_back("up");
		}
		if(col == 1)
		{
			str.push_back("right");
		}
		else if(col == -1)
		{
			str.push_back("left");
		}
	}

	return(str);
}

// Function that converts the player's input string to a set of coordinates on the grid.
Position Maze::StringToMove(Player *p, string input)
{
	// Gets the current position of that player on the grid.
	Position playerPos = p -> get_position();
	int playerRow = playerPos.row;
	int playerCol = playerPos.col;
	// desiredPos is where the user said they wanted to player to move to.
	Position desiredPos;
	// Convert the string direction to coordinates relative to the player's location.
	if(input == "up")
	{
		desiredPos.row = playerRow - 1;
		desiredPos.col = playerCol;
	}
	else if (input == "down")
	{
		desiredPos.row = playerRow + 1;
		desiredPos.col = playerCol;
	}
	else if (input == "right")
	{
		desiredPos.row = playerRow;
		desiredPos.col = playerCol + 1;
	}
	else if (input == "left")
	{
		desiredPos.row = playerRow;
		desiredPos.col = playerCol - 1;
	}
	return(desiredPos);
}

// Sets the location of the enemies when NewGame is called.
void Maze::SetPosition(Player* p)
{
	// Boolean value to record if the enemy has been placed on the board. Used because we may have to resample the random numbers 
	// multiple times if there is already something else at that location.
	bool isPlaced = false;

	// Continues to do this loop until the enemy has been placed.
	while(!isPlaced)
	{
		// Randomly generates some location within the grid.
		int randRow = rand() % board_ -> get_rows();
		int randCol = rand() % board_ -> get_cols();
		Position pos;
		pos.row = randRow;
		pos.col = randCol;
		// Only places the enemy there if that square is open. Otherwise repeats loop.
		if(board_ -> get_square_value(pos) == SquareType::Empty)
		{
			// Set the enemy's location on both the board and the enemy object.
			board_ -> SetSquareValue(pos, SquareType::Enemy);
			p -> SetPosition(pos);
			isPlaced = true;
		}
	}
}

// Called when the game ends to display scores.
void Maze::GenerateReport()
{
	// Print the name and points for each player.
	for(unsigned i = 0; i < players_.size(); i++)
	{
		cout << players_[i] -> get_name() << " has " << players_[i] -> get_points() << " points. ";
	}
	cout << endl;
}