#include "spawner.hpp"

Spawner::Spawner(
    EnemyTypes e_type,
    Rect rect,
    Level* level,
    int chance
) :
e_type(e_type),
rect(rect),
level(level),
chance(chance) { }

void Spawner::SpawnEnemyWithChance() const {
    if(rand() % 100 >= chance) return;
    Enemy e(&EnemyTypeList[e_type]);

    e.pos.x = rect.x + rand() % rect.w;
    e.pos.y = rect.y + rand() % rect.h;
    if (level->GetEnemy(e.pos) || !level->tiles[e.pos.x][e.pos.y].isPassable()) return;

    level->enemies.push_back(e);
}