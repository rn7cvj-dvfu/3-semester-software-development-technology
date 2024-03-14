#include "scroll_fire.hpp"

ScrollFire::ScrollFire(Level* level) : Spell::Spell(level) {
    this->type = Item::SPELL;
    this->colour = COL_PURPLE;
    this->symbol = 'N';
    this->_name = "Fire scroll";
}

void ScrollFire::ApplyEffects(Character *c) {
    ProjectileModifier* pm = new ProjectileModifier(_cur_level, 10);
    pm->NewColour(Colour::COL_RED);
    pm->IncreaseDam(10);
    std::function<void(expl_type, Point*)>* e_handler = new std::function<void(expl_type, Point*)>([pm](expl_type e, Point* p) {
        pm->cur_level->AddProjectile(new Projectile('+', Direction::NORTH, new Point(*p), Colour::COL_BLUE, 1));
        pm->cur_level->AddProjectile(new Projectile('+', Direction::SOUTH, new Point(*p), Colour::COL_BLUE, 1));
        pm->cur_level->AddProjectile(new Projectile('+', Direction::WEST, new Point(*p), Colour::COL_BLUE, 1));
        pm->cur_level->AddProjectile(new Projectile('+', Direction::EAST, new Point(*p), Colour::COL_BLUE, 1));
        GUI::SetStatus("Fire ball blasts with radiant light");
    });
    pm->NewOnExpl(e_handler);
    c->pm = pm;
}