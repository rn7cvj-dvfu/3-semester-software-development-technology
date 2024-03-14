#ifndef ENEMY_TYPE_H
#define ENEMY_TYPE_H

#include "geometry.hpp"

class EnemyType {
public:
    EnemyType(char symbol, Colour colour, int hp, int mv, int sight, int attack, std::string name);

    char symbol;
    Colour colour;
    int hp;
    int mv;
    int sight;
    int attack;
    std::string name;
};

static EnemyType EnemyTypeList[] = {
    EnemyType('z', COL_GREEN, 20, 1000, 10, 5, "Zombie"),
    EnemyType('s', COL_WHITE, 40, 2000, 10, 6, "Skeleton"),
    EnemyType('&', COL_PURPLE, 20, 600, 10, 9, "Shade")
};

enum EnemyTypes {
    ZOMBIE,
    SKELETON,
    SHADE,
    ENEMY_LAST_TYPE
};

#endif
