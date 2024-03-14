#include "healing_potion.hpp"
#include <sstream>

HealingPotion::HealingPotion(Level* level, HealingPotion::Potency inPot) : Spell::Spell(level) {
    const char* POTENCIES[HealingPotion::LAST_POTENCY] = {
        "Minor ",
        "Moderate ",
        "Strong "
    };
    
    std::string pot_name(POTENCIES[(int)inPot]);

    std::stringstream ss;
    ss << pot_name << "healing potion";

    this->potency = inPot;
    this->type = Item::SPELL;
    this->colour = COL_RED;
    this->symbol = 'U';
    this->_name = ss.str();
}

void
HealingPotion::ApplyEffects(Character *c) {
    int effect = 0;
    switch (potency)
    {
    case Potency::MINOR:
        effect = 10 + (rand() % 10);
        break;
    
    case Potency::MODERATE:
        effect = 30 + (rand() % 20);
        break;

    case Potency::STRONG:
        effect = 100;
        break;
    }
    c->Heal(effect);
}
