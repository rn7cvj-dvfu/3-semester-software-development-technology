#include "geometry.hpp"
#include "gui.hpp"
#include <sstream>
#include <ncurses.h>
#include <cstdlib>

Enemy::Enemy(void) {}

Enemy::Enemy(EnemyType *et) {
    this->parent_type = et;
    this->curHP = this->parent_type->hp;
    this->isActive = false;
    this->mv_energy = 0;
}

std::string
Enemy::Description(void) {
    std::stringstream ss;
    char c;

    c = this->parent_type->name[0];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        ss << "an ";
    else
        ss << "a ";
    ss << this->parent_type->name;
    return ss.str();
}

void
Enemy::Attack(Character *c) {
    GUI gui;
    std::stringstream ss;

    ss << this->parent_type->name << " attacks " << c->name;
    ss << " and cause " << c->TakeDamage(this->parent_type->attack) << " damage";

    gui.AddMessage(ss.str());
}

void
Enemy::TakeDamage(int amt) {
    this->curHP -= amt;
    this->isActive = true;
}

bool
Enemy::isAlive(void) {
    return curHP > 0;
}

char
Enemy::getSymbol(void) {
    return this->parent_type->symbol;
}

Colour
Enemy::getColour(void) {
    return this->parent_type->colour;
}
