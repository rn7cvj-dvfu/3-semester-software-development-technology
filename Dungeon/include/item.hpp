#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "geometry.hpp"

class Item {
public:
    enum Type {
        GENERIC,
        SPELL,
        TREASURE_T,
        EQUIPPABLE,
        LAST_TYPE
    };

    Item(void);
    ~Item(void);
    void SetPosition(int x, int y);
    std::string GetName(void);

    Type type;
    Point pos;
    char symbol;
    Colour colour;

protected:
    std::string _name;
};

#endif
