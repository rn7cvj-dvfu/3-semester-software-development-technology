#include "equippable.hpp"
#include "gui.hpp"
#include "geometry.hpp"
#include <sstream>

Equippable::Equippable() {
    this->_attackMod = 0;
    this->_defMod = 0;
    this->colour = COL_CYAN;
    this->type = Item::EQUIPPABLE;
}

int Equippable::AttackMod() const { return this->_attackMod; }

int Equippable::DefMode() const { return this->_defMod; }

Sword::Sword() {
    this->_attackMod = 10;
    this->_defMod = 10;
    this->_name = std::string("Sword");
    this->symbol = 'l';
}

Axe::Axe() {
    this->_attackMod = 20;
    this->_defMod = 0;
    this->_name = std::string("Axe");
    this->symbol = 'p';
}

Shield::Shield() {
    this->_attackMod = 0;
    this->_defMod = 30;
    this->_name = std::string("Shield");
    this->symbol = '0';
}

Helmet::Helmet() {
    this->_attackMod = 0;
    this->_defMod = 10;
    this->_name = std::string("Helmet");
    this->symbol = 'm';
}

Equippable* EquippableFactory::Get(EquippableName name) const { 
    switch (name)
    {
    case SWORD:
        return new Sword();
        break;
    case AXE:
        return new Axe();
        break;
    case SHIELD:
        return new Shield();
        break;
    case HELMET:
        return new Helmet();
        break;
    default:
        throw std::logic_error("Wrong equippable name");
        break;
    }
}

Equippable* EquippableFactory::GetRandom() const {
    int rand_int = rand() % EquippableName::LAST_ITEM;
    return Get((EquippableName)rand_int);
}