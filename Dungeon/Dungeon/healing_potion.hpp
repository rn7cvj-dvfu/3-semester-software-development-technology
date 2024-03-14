#ifndef HEALINGPOTION_H
#define HEALINGPOTION_H

#include "character.hpp"
#include "item.hpp"
#include <string>

class Character;

class HealingPotion : public Spell {
public:
    enum Potency {
        MINOR,
        MODERATE,
        STRONG,
        LAST_POTENCY
    };

    HealingPotion(Level* level, HealingPotion::Potency inPot);

    void ApplyEffects(Character *c) override;

    Potency potency;

};

#endif
