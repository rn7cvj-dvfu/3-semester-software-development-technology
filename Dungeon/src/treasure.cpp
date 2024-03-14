#include "treasure.hpp"
#include <sstream>

Treasure::Treasure(int n) {
    std::stringstream ss;

    this->type = Item::TREASURE_T;
    this->quantity = n;

    this->symbol = '$';
    this->colour = COL_YELLOW;

    ss << n << " gold pieces";
    this->_name = ss.str();
}

int Treasure::Quantity() const {
    return quantity;
}