#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include <string>
#include <time.h>

class Equippable : public Item {
public:
    Equippable();
    int AttackMod() const;
    int DefMode() const;

protected:
    int _attackMod;
    int _defMod;

};

class Sword : public Equippable {
public:
    Sword();
};

class Axe : public Equippable {
public:
    Axe();
};

class Shield : public Equippable {
public:
    Shield();
};

class Helmet : public Equippable {
public:
    Helmet();
};

class EquippableFactory {
protected:
    enum EquippableName {
        SWORD,
        AXE,
        SHIELD,
        HELMET,
        LAST_ITEM,
    };

public:
    Equippable* Get(EquippableName name) const;
    Equippable* GetRandom() const;
};

#endif
