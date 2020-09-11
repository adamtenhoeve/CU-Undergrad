#ifndef _ITEM_H_
#define _ITEM_H_

/** 
  Provided code for CSCI 3010, HW 2, Spring 2019
*/

#include <string>

class Item {
public:
  Item(int id, std::string type, double cost, int initial_quantity)
  : id_(id),
  type_(type),
  cost_(cost),
  quantity_(initial_quantity)
  {}

  // Return the item id
  int get_id() { return id_; };
  // Return the number of that item, either in the store or in the shopping cart
  int get_quantity() { return quantity_; };
  // Return the cost of the item
  double get_cost() { return cost_; };
  // Returns the "name" of the item
  std::string get_type() { return type_; };
  // Increases the number by the given input
  void IncreaseQuantity(int amount) { quantity_ += amount; };
  // Decreases the number by the given input
  void DecreaseQuantity(int amount) { quantity_ -= amount; };
  // Returns a string of the name, cost and quantity of the item
  std::string ToString() const;
  // Creates a duplicate of the object (same id, name and cost) but with a quantity of 1.
  Item* Clone() { return new Item(this->id_, this->type_, this->cost_, 1); };
  
  friend std::ostream& operator<<(std::ostream& os_lhs, const Item& i);
private:
  int id_;
  std::string type_;
  double cost_;
  int quantity_;
};


#endif  // _ITEM_H_