#ifndef ENEMY_H
#define ENEMY_H

#include "enemy_type.hpp"
#include "character.hpp"
#include <string>

class Character;

class Enemy {
public:
    Enemy();
    Enemy(EnemyType *et);
    std::string Description();
    void Attack(Character *c);
    void TakeDamage(int amt);
    bool isAlive();
    char getSymbol();
    Colour getColour();

    EnemyType *parent_type;
    int curHP;
    bool isActive;
    Point pos;
    int mv_energy;
};

#endif
