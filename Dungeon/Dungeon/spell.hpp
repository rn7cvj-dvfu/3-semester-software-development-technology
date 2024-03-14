#ifndef SPELL_H
#define SPELL_H

#include "character.hpp"
#include "item.hpp"
#include <string>
#include "level.hpp"

class Character;
class Level;

class Spell : public Item {
protected:
    Level* _cur_level;
    
public:
    Spell(Level* level);

    virtual void ApplyEffects(Character *c) = 0;

};

#endif
