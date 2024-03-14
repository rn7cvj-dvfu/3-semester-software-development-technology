#ifndef SCROLLFIRE_H
#define SCROLLFIRE_H

#include "spell.hpp"
#include "projectile_modifier.hpp"

#include "gui.hpp"

class ScrollFire : public Spell {
public:
    ScrollFire(Level* level);
    void ApplyEffects(Character *c);
};

#endif