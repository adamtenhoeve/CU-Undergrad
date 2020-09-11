#include <iostream>
#include <iomanip>
#include "Item.h"
#include "Store.h"
#include "TextUI.h"

// Name: Adam Ten Hoeve

using namespace std;

int main(int argc, char* argv[])
{
	// Check that there are 2 inputs on the command line
	if(argc != 2) { return -1;}

	// Declare TextUI, Store and Shopping Cart classes
	TextUI *ui = new TextUI();
	Store *store = new Store(argv[1]);

	// Start the shopping loop. Continues until they decide to leave
	string choice = "";
	while(choice != "Leave")
	{
		// Display the main menu for the customer to select a choice
		choice = ui -> MainMenu();
		// Then send selected choice to function that determines how class operates.
		ui -> RouteChoice(*store, choice);
	}
	// Once the customer has left the store, delete the ui and store objects and return,
	delete(ui);
	delete(store);
	return 0; 
}
