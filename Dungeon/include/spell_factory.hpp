#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include "scroll_fire.hpp"
#include "healing_potion.hpp"

class Level;

class SpellFactory {
public:
    SpellFactory(Level* level);
    Spell* GetRandomSpell() const;

private:
    Level* _cur_level;

};

#endif