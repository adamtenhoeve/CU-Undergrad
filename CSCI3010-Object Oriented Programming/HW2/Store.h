#ifndef _STORE_H_
#define _STORE_H_

/** 
  Provided code for CSCI 3010, HW 2, Spring 2019
*/

#include <vector>
#include <map>
#include <fstream>

#include "Item.h"


class ShoppingCart {
public:
  // Add a single item to the cart
  void AddItem(Item * to_add);
  // Remove a single item from the cart
  void RemoveItem(Item * to_remove);
  // Displays the stringified version of all items in the cart
  std::string DisplayCart();
  // Remove all items from the cart
  void ClearCart() { in_cart_.clear(); };
  // Get the item vector, not just the stringified version.
  std::vector<Item *> get_items() { return in_cart_; };

private:
  std::vector<Item *> in_cart_;
};


class Store {
public:
  // Constructor to read in the file and load all of the store data
  Store(std::string filename);
  // Display the string versions of all items in the shop
  std::string DisplayInventory();
  // Returns the id and stringified items in the shop
  std::map<int, std::string> Items();
  // Returns the id and stringified items in the cart
  std::map<int, std::string> CartItems();
  // Take an item out of the shop (reduce quantity by 1) and put into cart (increase quantity by 1)
  void AddItemToCart(int item_id);
  // Take an item out of the cart
  void RemoveItemFromCart(int item_id);
  // Calls the DisplayCart function in the ShoppingCart class
  std::string DisplayCart();
  // Convert the total cost given the number of items purchased and the cost of each.
  double Checkout();
  // Removes all items from the cart
  void ClearCart();
  // Returns the name of the store.
  std::string get_name() const {return name_;};


private:
  std::string name_;
  std::string filename_;
  std::vector<Item *> inventory_;

  ShoppingCart * cart_;
};


#endif  // _STORE_H_