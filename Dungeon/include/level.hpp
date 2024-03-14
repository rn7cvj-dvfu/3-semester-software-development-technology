#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <list>
#include "tile.hpp"
#include "geometry.hpp"
#include "character.hpp"
#include "game.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "treasure.hpp"
#include "projectile.hpp"
#include "bomb.hpp"
#include "spawner.hpp"

#define MAP_W   100
#define MAP_H   50

typedef Rect Room;

class Game;
class Spawner;
class Spell;

struct Corridor {
    Rect pos;
    Direction::Type direction;
};

class Level {
public:
    Level(Level* parent);
    bool TileIsVisible(Point p);
    void ConditionallyShowObject(Point p, char c, int col);
    void RevealSight(Point p, int distance);
    void DoSightBeam(Direction::Type d, int x, int y, float ttl);
    void GiveEnemiesTurns(Character *c, int n);
    void EnemyAdvance(Enemy *e, Character *c);

    void DoAttack(Enemy *e, int dam);
    void RevealAll(void);
    bool CanSee(Point p1, Point p2);
    void ClearObstacles(void);

    void AddEquippable(Rect *r);
    void AddSpell(Rect *r);
    void AddBomb(Rect *r);
    void EnemySpawn(Rect *r);
    void AddSpawner(Rect* r);
    void AddMutBlock(Rect* r);

    void AddSpellToPlace(Point p, Spell* spell);
    Direction::Type GetInputDir();

    void PlaceMutableBlock(Point p);

    void RemoveBomb(Bomb* bomb);
    void RemoveEnemy(Enemy *e);

    Enemy* GetEnemy(const Point& p);
    Item* GetItem(const Point& p);
    Bomb* GetBomb(const Point& p);

    void AddProjectile(Projectile* proj);
    void MoveProjectiles();

    ~Level();

    Tile                tiles[MAP_W][MAP_H];
    std::list<Item*>    items;
    std::list<Enemy>    enemies;
    std::vector<Projectile*> projs;
    std::vector<Bomb*> bombs;
    std::vector<Spawner*> spawners;
    Point start_point;

private:
    void ApplyRoom(Room * const r, bool isFirstRoom);
    void ApplyCorridor(Corridor const * const c);
    bool CorridorFits(Corridor const * const c);
    bool RoomFits(Room const * const r);
    Corridor FindRoomCorridorChild(Room *r);
    Room RoomFromCorridor(Corridor *c);
    void AddGold(Rect *r);
    void AddPillars(Rect *r);
};

#endif
