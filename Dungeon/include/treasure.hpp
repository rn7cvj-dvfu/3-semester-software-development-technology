#ifndef TREASURE_H
#define TREASURE_H

#include <string>
#include "item.hpp"

class Treasure : public Item {
public:
    Treasure(int n);

    int Quantity() const;

private:
    int quantity;

};

#endif
