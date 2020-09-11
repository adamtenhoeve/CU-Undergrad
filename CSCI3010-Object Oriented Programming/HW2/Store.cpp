#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <math.h>

#include "Item.h"
#include "Store.h"

/** 
  Provided code for CSCI 3010, HW 2, Spring 2019
*/
// Displays the stringified version of all Items in the cart
std::string ShoppingCart::DisplayCart() {
  std::string inv = "";
  // Loop over all items in the cart
  for (Item * i : in_cart_) {
    // Adds the stringified version to a string
    inv.append(i->ToString());
    inv.append("\n");
  }
  // Returns the larger string
  return inv;
}
// Removes single item from the cart
void ShoppingCart::RemoveItem(Item * to_remove) {
  // Reduce the quantity of the item by 1. Only happens if more than one in the cart.
  if (to_remove->get_quantity() > 1) {
    to_remove->DecreaseQuantity(1);
    // Otherwise need to remove the Item from the cart because quantity = 0.
  } else {
    int target = -1;
    for (int i = 0; i < in_cart_.size(); i++) {
      if (in_cart_[i] == to_remove) {
        target = i;
        break;
      }
    }
    if (target >= 0) {
      in_cart_.erase(in_cart_.begin() + target);
    }
  }
}
// Add an item to the cart
void ShoppingCart::AddItem(Item * to_add) {
  bool added = false;
  // Sees if the item is already in the cart
  for (int i = 0; i < in_cart_.size(); i++) {
    // If it is, increases the quantity of the item by 1.
    if (in_cart_[i]->get_id() == to_add->get_id()) {
      in_cart_[i]->IncreaseQuantity(1);
      added = true;
      break;
    }
  }
  // If it isn't, then adds it to the end of the cart's Item vector.
  if (!added) {
    in_cart_.push_back(to_add);
  }
}

// Store Constructor. Reads a file and initializes all the data from the input filename.
Store::Store(std::string filename) {
  filename_ = filename;
  // Open the file
  std::ifstream file;
  if (! file.is_open()) {
    file.open(filename);
  }
  // Initialize a bunch of stuff. Specifically a string to store all the strings of all added items.
  std::string line;
  std::string inv = "Inventory:";
  bool inventory = false;
  int id = 1;
  // Loop over entire opened file and store in the Store's Item vector.
  while (std::getline(file, line)) {
    // name is the first line
    if (name_.size() == 0) {
      name_ = line;
    }
    if (line.substr(0, inv.size()) == inv) {
      inventory = true;
    } else if (inventory) {
      std::vector<std::string> tokens;
      std::istringstream iss(line);
      std::string token;
      while(std::getline(iss, token, ',')) {
        tokens.push_back(token);
      }
      Item * i1 = new Item(id, tokens[0], std::stod(tokens[1]), std::stoi(tokens[2]));
      inventory_.push_back(i1);
      id++;
    }
  }
  // Always close your files.
  file.close();

  // Initialize an empty shopping cart for the customer.
  cart_ = new ShoppingCart();
}
// Returns a map of each items id and the stringify version.
std::map<int, std::string> Store::Items() {
  std::map<int, std::string> items;
  // Loops over all items in store. Gets the id and string of each and adds it to a map.
  for (Item * i : inventory_) {
    int id = i->get_id();
    std::string as_str = i->ToString();
    if (i->get_quantity() > 0) {
      items.insert({id, as_str});
    }
  }
  // Return the map
  return items;
}
// The same as above function but loops over the shopping cart items rather than store items.
std::map<int, std::string> Store::CartItems() {
  std::map<int, std::string> items;
  for (Item * i : cart_->get_items()) {
    int id = i->get_id();
    std::string as_str = i->ToString();
    if (i->get_quantity() > 0) {
      items.insert({id, as_str});
    }
  }
  return items;
}
// Decrease an item's quantity in the store by 1 and but it into the shopping cart/increase it's quantity by 1.
void Store::AddItemToCart(int item_id) {
  Item * target;
  for (Item * i : inventory_) {
    if (i->get_id() == item_id) {
      i->DecreaseQuantity(1);
      // Use clone to create new version of item with quantity 1 for the shopping cart.
      target = i->Clone();
      break;
    }
  }
  // Add the item to the cart
  cart_->AddItem(target);
}
// Take given item out of the cart
void Store::RemoveItemFromCart(int item_id) {
  Item * target;
  // Loop over entire list of items to find the one to remove
  for (Item * i : cart_->get_items()) {
    if (i->get_id() == item_id) {
      target = i;
      break;
    }
  }
  // Remove the item from the cart using the Cart's functions.
  cart_->RemoveItem(target);
  // Add one back to the quantity of the item in the store.
  for (Item * i : inventory_) {
    if (i->get_id() == item_id) {
      i->IncreaseQuantity(1);
    }
  }
}
// Display stringified items in cart. Same as DisplayCart in cart class.
std::string Store::DisplayCart() {
  return cart_->DisplayCart();
}

// Display stringified items in store.
std::string Store::DisplayInventory() {
  std::string inv = "";
  // Loop over entire store inventory and add stringified versions to larger string.
  for (Item * i : inventory_) {
    inv.append(i->ToString());
    inv.append("\n");
  }
  return inv;
}
// Calculate the cost of all items in cart given quantity and price.
double Store::Checkout() {
  // Loop through entire cart and calculate total price, with tax.
  double total = 0.0;
  for (Item * i : cart_->get_items()) {
    // tax rate for 80309
    total += (i->get_cost() * i->get_quantity()) * 1.08845;
  }

  // then we will overwrite the file
  std::ofstream ofs;
  ofs.open (filename_, std::ofstream::out);
  // Rewrites the quantities of the file. This was super annoying during testing.
  ofs << name_ << std::endl << std::endl;
  ofs << "Inventory:" << std::endl;
  for (Item * i : inventory_) {
    ofs << i->get_type() << "," << std::to_string(roundf(i->get_cost() * 100) / 100)
     << "," << std::to_string(i->get_quantity()) << std::endl;
  }
  ofs.close();
  // Remove all items from the cart
  this->ClearCart();

  return total;
}
// Remove all the items from the cart. Same as the ClearCart function in the ShoppingCart class.
void Store::ClearCart() {
  cart_->ClearCart();
}

