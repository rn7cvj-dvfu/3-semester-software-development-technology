#ifndef SPAWNER_H
#define SPAWNER_H

#include "geometry.hpp"
#include "enemy_type.hpp"
#include "level.hpp"

class Level;

class Spawner {
public:
    EnemyTypes e_type;
    Rect rect;
    Level* level;
    int chance;

    Spawner(EnemyTypes e_type, Rect rect, Level* level, int chance);

    void SpawnEnemyWithChance() const;
};

#endif