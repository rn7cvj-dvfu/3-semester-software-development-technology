#include "spell_factory.hpp"

SpellFactory::SpellFactory(Level* level) : _cur_level(level) { }

Spell* SpellFactory::GetRandomSpell() const {
    int rand_int = rand() % 4;
    switch (rand_int)
    {
    case 0:
        return new ScrollFire(_cur_level);    
    case 1:
        return new HealingPotion(_cur_level, HealingPotion::Potency::MINOR);
    case 2:
        return new HealingPotion(_cur_level, HealingPotion::Potency::MODERATE);
    case 3:
        return new HealingPotion(_cur_level, HealingPotion::Potency::STRONG);
    default:
        return new HealingPotion(_cur_level, HealingPotion::Potency::MINOR);
    }
}