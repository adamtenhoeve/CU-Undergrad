#include <iostream>
#include <string>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <math.h>

#include "Item.h"

/** 
  Provided code for CSCI 3010, HW 2, Spring 2019
*/

// Returns the stringified combination of the name, price and quantitiy of an item.
std::string Item::ToString() const {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) << cost_;
  std::string s = stream.str();
  return type_ + ": " + s + " - " + std::to_string(quantity_);
}
// Friend operator to print the ToString version of Item.
std::ostream& operator<<(std::ostream& os_lhs, const Item& i) {
  os_lhs << i.ToString();
  return os_lhs;
}