#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Item.h"
#include "Store.h"
#include "TextUI.h"

#include <vector>

using namespace std;

// Name: Adam Ten Hoeve

// First test case checks the basic functionallity of the Item class
TEST_CASE ("Checking functions of the Item class", "[Item]")
{
	// Declare 3 basic items
	Item i1(1, "cheese", 10.25, 4);
	Item i2(2, "butter", 4.5, 6);
	Item i3(3, "Moose", 20, 1);

	// Test all the get functions so that they return the correct, associated variable.
	SECTION("Testing get_id")
	{
		REQUIRE(i1.get_id() == 1);
		REQUIRE(i2.get_id() == 2);
		REQUIRE(i3.get_id() == 3);
	}

	SECTION("Testing get_quantity")
	{
		REQUIRE(i1.get_quantity() == 4);
		REQUIRE(i2.get_quantity() == 6);
		REQUIRE(i3.get_quantity() == 1);
	}

	SECTION("Testing get_cost")
	{
		REQUIRE(i1.get_cost() == 10.25);
		REQUIRE(i2.get_cost() == 4.5);
		REQUIRE(i3.get_cost() == 20);
	}

	SECTION("Testing get_type")
	{
		REQUIRE(i1.get_type() == "cheese");
		REQUIRE(i2.get_type() == "butter");
		REQUIRE(i3.get_type() == "Moose");
	}

	// Test the IncreaseQuantity funciton to make sure that it increase from where it was by the set amount.
	SECTION("Testing IncreaseQuantity")
	{
		i1.IncreaseQuantity(3);
		REQUIRE(i1.get_quantity() == 7);
		i1.IncreaseQuantity(4);
		REQUIRE(i1.get_quantity() == 11);
	}

	// Make sure the DecreaseQuantity function decrease the item quantity from where it was by the set amount.
	SECTION("Testing DecreaseQuantity")
	{
		i1.DecreaseQuantity(3);
		REQUIRE(i1.get_quantity() == 1);
		i1.DecreaseQuantity(4);
		REQUIRE(i1.get_quantity() == -3);
		i3.DecreaseQuantity(1);
		REQUIRE(i3.get_quantity() == 0);
	}

	// Makes sure it has the right output for the text output used in the other classes.
	SECTION("Testing ToString")
	{
		REQUIRE(i1.ToString() == "cheese: 10.25 - 4");
		REQUIRE(i2.ToString() == "butter: 4.50 - 6");
		REQUIRE(i3.ToString() == "Moose: 20.00 - 1");
	}

	// Makes clones of one of the items, then checks its output string to make sure it is the same but with a quantity of 1.
	SECTION("Testing Clone")
	{
		Item *temp = i1.Clone();
		REQUIRE(temp -> ToString() == "cheese: 10.25 - 1");
		Item *temp2 = i3.Clone();
		REQUIRE(temp2 -> ToString() == "Moose: 20.00 - 1");
	}
}


//////////////////////////    TESTING SHOPPINGCART CLASS ///////////////////////////////////////////////////////////////

// Testing the add and remove functions. Do so with the displaycart function.
TEST_CASE("Testing the DisplayCart function in ShoppingCart", "[ShoppingCart]")
{
	// Declare 3 items used above and a shopping cart.
	Item i1(1, "cheese", 10.25, 4);
	Item i2(2, "butter", 4.5, 6);
	Item i3(3, "Moose", 20, 1);
	ShoppingCart sc;
	
	// Add items to the shopping cart and guarantee that they are there in the correct amounts.
	SECTION("Testing AddItem and DisplayCart")
	{
		REQUIRE(sc.DisplayCart() == "");
		sc.AddItem(&i1);
		REQUIRE(sc.DisplayCart() == "cheese: 10.25 - 4\n");
		sc.AddItem(&i2);
		REQUIRE(sc.DisplayCart() == "cheese: 10.25 - 4\nbutter: 4.50 - 6\n");
	}

	// Remove items from the shopping cart and make sure amounts stay consistent
	SECTION("Testing RemoveItem and DisplayCart")
	{
		REQUIRE(sc.DisplayCart() == "");
		sc.RemoveItem(&i1);
		REQUIRE(sc.DisplayCart() == "");
		sc.AddItem(&i1);
		sc.AddItem(&i2);
		REQUIRE(sc.DisplayCart() == "cheese: 10.25 - 3\nbutter: 4.50 - 6\n");
		sc.RemoveItem(&i2);
		REQUIRE(sc.DisplayCart() == "cheese: 10.25 - 3\nbutter: 4.50 - 5\n");
	}
}

// Make sure the clearcart function works by adding items to the cart, then calling the function and checking if the cart is empty.
TEST_CASE("Testing the ClearCart function in ShoppingCart", "[ShoppingCart]")
{
	Item i1(1, "cheese", 10.25, 4);
	Item i2(2, "butter", 4.5, 6);
	Item i3(3, "Moose", 20, 1);
	ShoppingCart sc;

	sc.AddItem(&i1);
	sc.AddItem(&i2);
	sc.AddItem(&i3);

	REQUIRE(sc.DisplayCart() == "cheese: 10.25 - 4\nbutter: 4.50 - 6\nMoose: 20.00 - 1\n");

	sc.ClearCart();

	REQUIRE(sc.DisplayCart() == "");
}

// Check that the get_items function returns the correct items in a vector of items
TEST_CASE("Testing the get_items() function in ShoppingCart", "[ShoppingCart]")
{
	Item i1(1, "cheese", 10.25, 4);
	Item i2(2, "butter", 4.5, 6);
	Item i3(3, "Moose", 20, 1);
	ShoppingCart sc;

	vector<Item*> temp;
	vector<Item*> test;

	temp = sc.get_items();
	REQUIRE(temp.empty());

	sc.AddItem(&i1);
	test.push_back(&i1);
	sc.AddItem(&i2);
	test.push_back(&i2);

	REQUIRE(sc.get_items() == test);

	sc.RemoveItem(&i1);
	REQUIRE(sc.get_items() == test);
}


//////////////////////////////////// STORE TEST CASES ////////////////////////////////////////////////////////
// Make sure the DisplayInventory displays all items in the correct syntax.
TEST_CASE("Testing DisplayInventory for Store class")
{
	string filename = "store.txt";
	Store store(filename);

	REQUIRE(store.DisplayInventory() == "Tea: 2.00 - 6\nTall Candle: 7.00 - 13\nKettle: 15.00 - 1\n");
}

// Check that the Items() function retuns a map of the same items in the proper order.
TEST_CASE("Testing Items() function in the Store Class")
{
	string filename = "store.txt";
	Store store(filename);

	map<int, string> testMap;
	testMap.insert({1, "Tea: 2.00 - 6"});
	testMap.insert({2, "Tall Candle: 7.00 - 13"});
	testMap.insert({3, "Kettle: 15.00 - 1"});
	REQUIRE(store.Items() == testMap);
}

// Seeing if the interfunctionallity between the Store and ShoppingCart classes works.
TEST_CASE("Testing AddItemToCart, RemoveItemFromCart and CartItems functions in the Store Class")
{
	string filename = "store.txt";
	Store store(filename);

	store.AddItemToCart(1);
	store.AddItemToCart(2);
	// Check to make sure items added through Store are in shopping cart.
	REQUIRE(store.DisplayInventory() == "Tea: 2.00 - 5\nTall Candle: 7.00 - 12\nKettle: 15.00 - 1\n");

	store.AddItemToCart(3);

	REQUIRE(store.DisplayInventory() == "Tea: 2.00 - 5\nTall Candle: 7.00 - 12\nKettle: 15.00 - 0\n");

	map<int, string> testMap;
	testMap.insert({1, "Tea: 2.00 - 1"});
	testMap.insert({2, "Tall Candle: 7.00 - 1"});
	testMap.insert({3, "Kettle: 15.00 - 1"});
	// Check that the items are the correct items, with the correct amounts and syntax.
	REQUIRE(store.CartItems() == testMap);
	// Remove items from the cart and check that everything is still hunky-dory.
	store.RemoveItemFromCart(1);
	store.RemoveItemFromCart(3);
	testMap.erase(1);
	testMap.erase(3);

	REQUIRE(store.CartItems() == testMap);
}

// Checking if the Store is able to display all the items in the cart and remove all of the items in the cart.
TEST_CASE("Testing DisplayCart and ClearCart functions in Store class", "[Store]")
{
	string filename = "store.txt";
	Store store(filename);

	REQUIRE(store.DisplayCart() == "");

	// Add initial items to the cart
	store.AddItemToCart(1);
	store.AddItemToCart(2);
	REQUIRE(store.DisplayCart() == "Tea: 2.00 - 1\nTall Candle: 7.00 - 1\n");
	// Add the second of an item to the cart
	store.AddItemToCart(1);
	REQUIRE(store.DisplayCart() == "Tea: 2.00 - 2\nTall Candle: 7.00 - 1\n");

	store.ClearCart();
	REQUIRE(store.DisplayCart() == "");
}

// Does the function correctly calculate the price of multiple items.
TEST_CASE("Testing Checkout function in Store class", "[Store]")
{
	string filename = "store.txt";
	Store store(filename);
	// Test basic functionality.
	store.AddItemToCart(1);
	REQUIRE(store.Checkout() == 2.1769);
	// Test multiple of the same item.
	store.AddItemToCart(1);
	store.AddItemToCart(2);
	store.AddItemToCart(2);
	REQUIRE(store.Checkout() == 17.4152);
}

// Make sure that the class returns the correct name of the store, given in the text file.
TEST_CASE("Testing get_name function in Store class", "[Store]")
{
	string filename = "store.txt";
	Store store(filename);

	REQUIRE(store.get_name() == "My Store");
}