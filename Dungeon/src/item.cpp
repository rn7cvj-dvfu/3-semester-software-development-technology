#include "item.hpp"
#include "spell.hpp"
#include "treasure.hpp"
#include "equippable.hpp"

Item::Item(void) {
    this->type = Item::GENERIC;
}

void
Item::SetPosition(int x, int y) {
    this->pos.x = x;
    this->pos.y = y;
}

std::string
Item::GetName(void) {
    return this->_name;
}

Item::~Item(void) {
}
