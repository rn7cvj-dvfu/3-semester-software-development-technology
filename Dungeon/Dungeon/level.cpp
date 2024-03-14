#include "level.hpp"
#include "gui.hpp"
#include "equippable.hpp"
#include "enemy_type.hpp"
#include <cstdlib>
#include <sstream>
#include "healing_potion.hpp"
#include "spell_factory.hpp"
#include "bomb.hpp"
#include "mutable_block.hpp"

#define CORRIDOR_TRIES      30
#define TTL_MAX             10

Level::Level(Level* parent) {
    Room r;

    for (int e = 0; e < MAP_H; ++e) {
        for (int i = 0; i < MAP_W; ++i) {
            this->tiles[i][e] = Tile(false, WALL_CHAR);
        }
    }

    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;
    r.x = rand() % (MAP_W - r.w - 2) + 1;
    r.y = rand() % (MAP_H - r.h - 2) + 1;

    this->start_point.x = r.x + rand() % (r.w - 2) + 1;
    this->start_point.y = r.y + rand() % (r.h - 2) + 1;

    projs.reserve(20);

    this->ApplyRoom(&r, true);
}

void
Level::ApplyRoom(Room * const r, bool isFirstRoom) {
    Corridor c;
    Room r_child;
    int exits;

    exits = 0;

    for (int e = 0; e < r->h; ++e) {
        for (int i = 0; i < r->w; ++i) {
            this->tiles[ i + r->x ][ e + r->y ].setTileType(FLOOR_CHAR);
        }
    }

    if (!isFirstRoom) { 
        if (rand() % 100 < 5)
            this->AddPillars(r);

        if (rand() % 100 < 10)
            this->AddSpawner(r);

        if (rand() % 100 < 20)
            this->AddGold(r);

        if (rand() % 100 < 20)
            this->AddSpell(r);

        if (rand() % 100 < 10)
            this->AddEquippable(r);

        if (rand() % 100 < 80)
            this->EnemySpawn(r);

        if (rand() % 100 < 20)
            this->AddMutBlock(r);

        if (rand() % 100 < 20)
            this->AddBomb(r);
    }

    for (int i = 0; i < CORRIDOR_TRIES; ++i) {
        c = this->FindRoomCorridorChild(r);
        if (!this->CorridorFits(&c))
            continue;
        r_child = this->RoomFromCorridor(&c);
        if (!this->RoomFits(&r_child))
            continue;

        exits++;
        this->ApplyCorridor(&c);
        this->ApplyRoom(&r_child, false);
    }
}

bool
Level::CorridorFits(Corridor const * const c) {
    Rect r;

    if (c->pos.x <= 1 || c->pos.y <= 1)
        return false;
    if (c->pos.x + c->pos.w >= MAP_W)
        return false;
    if (c->pos.y + c->pos.h >= MAP_H)
        return false;

    r = c->pos;
    if (c->direction == Direction::EAST)
        r.x ++;
    else if (c->direction == Direction::SOUTH)
        r.y ++;

    for (int e = MAX(r.y - 1, 0); e <= r.y + r.h; ++e) {
        for (int i = MAX(r.x - 1, 0); i <= r.x + r.w; ++i) {
            if (this->tiles[i][e].getTileType() == FLOOR_CHAR)
                return false;
        }
    }

    return true;
}

bool
Level::RoomFits(Room const * const r) {
    if (r->x <= 0 || r->y <= 0)
        return false;
    if (r->x + r->w >= MAP_W)
        return false;
    if (r->y + r->h >= MAP_H)
        return false;

    for (int e = MAX(r->y - 1, 0); e <= r->y + r->h; ++e) {
        for (int i = MAX(r->x - 1, 0); i <= r->x + r->w; ++i) {
            if (this->tiles[i][e].getTileType() == FLOOR_CHAR)
                return false;
        }
    }

    return true;
}

void
Level::ApplyCorridor(Corridor const * const c) {
    Rect r;
    Direction::Type d;

    r = c->pos;
    d = c->direction;

    this->tiles[r.x][r.y].setTileType(CLOSED_DOOR_CHAR);
    this->tiles[r.x + r.w][r.y + r.h].setTileType(CLOSED_DOOR_CHAR);
    if (d == Direction::EAST || d == Direction::WEST) {
        for (int i = 1; i < r.w; ++i)
            this->tiles[i + r.x][r.y].setTileType(FLOOR_CHAR);
    }
    else {
        for (int i = 1; i < r.h; ++i)
            this->tiles[r.x][i + r.y].setTileType(FLOOR_CHAR);
    }
}

Room
Level::RoomFromCorridor(Corridor *c) {
    Room r;
    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;

    // Only add-sized rooms
    if (r.w % 2 == 0)
        r.w++;
    if (r.h % 2 == 0)
        r.h++;

    if (c->direction == Direction::NORTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y - r.h;
    }
    else if (c->direction == Direction::SOUTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y + c->pos.h + 1;
    }
    else if (c->direction == Direction::EAST) {
        r.x = c->pos.x + c->pos.w + 1;
        r.y = c->pos.y - rand() % r.h;
    }
    else { //WEST
        r.x = c->pos.x - r.w;
        r.y = c->pos.y - rand() % r.h;
    }

    return r;
}

Corridor
Level::FindRoomCorridorChild(Room *r) {
    Corridor c;

    if (rand() % 2) { //NORTH OR SOUTH
        c.pos.w = 0;
        c.pos.h = ((rand() % 2) + 1) * 3;
        c.pos.x = r->x + rand() % r->w;
        if (rand() % 2) { //NORTH
            c.direction = Direction::NORTH;
            c.pos.y = r->y - 1 - c.pos.h;
        }
        else { //SOUTH
            c.direction = Direction::SOUTH;
            c.pos.y = r->y + r->h;
        }
    }
    else { //EAST OR WEST
        c.pos.h = 0;
        c.pos.w = ((rand() % 3) + 1) * 3;
        c.pos.y = r->y + rand() % r->h;
        if (rand() % 2) { // WEST
            c.direction = Direction::WEST;
            c.pos.x = r->x - 1 - c.pos.w;
        }
        else { //EAST
            c.direction = Direction::EAST;
            c.pos.x = r->x + r->w;
        }
    }

    return c;
}

bool
Level::TileIsVisible(Point p) {
    return this->tiles[p.x][p.y].isVisible;
}

void
Level::ConditionallyShowObject(Point p, char c, int col) {
    if (!GUI::isOnScreen(p))
        return;
    if (!this->TileIsVisible(p))
        return;
    attron(COLOR_PAIR(col));
    GUI::DrawObjectRelative(p, c);
    attroff(COLOR_PAIR(col));
}

void
Level::RevealSight(Point p, int distance) {
    DoSightBeam(Direction::NORTH, p.x, p.y, distance);
    DoSightBeam(Direction::SOUTH, p.x, p.y, distance);
    DoSightBeam(Direction::EAST, p.x, p.y, distance);
    DoSightBeam(Direction::WEST, p.x, p.y, distance);
}

void
Level::DoSightBeam(Direction::Type d, int x, int y, float ttl) {
    Tile *t;

    if (ttl < 0)
        return;
    t = &this->tiles[x][y];
    t->isVisible = true;
    if (!t->isSeeThrough())
        return;

    if (d == Direction::NORTH) {
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::NORTH, x, y - 1, ttl - 1);
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
    }
    else if (d == Direction::SOUTH) {
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
        DoSightBeam(Direction::SOUTH, x, y + 1, ttl - 1);
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::EAST) {
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::EAST, x + 1, y, ttl - 1);
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::WEST) {
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::WEST, x - 1, y, ttl - 1);
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::NW)
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
    else if (d == Direction::NE)
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
    else if (d == Direction::SW)
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
    else if (d == Direction::SE)
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
}

void
Level::AddGold(Rect *r) {
    Treasure *tres;
    int quantity;
    int TTL;
    Point p;

    quantity = rand() % 10;

    TTL = 5;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL-- <= 0)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable());

    if (TTL == 0)
        return;

    tres = new Treasure(quantity);
    tres->SetPosition(p.x, p.y);

    this->items.push_back((Item*) tres);

    if (rand() % 100 <= 10)
        this->AddGold(r);
}

void
Level::AddSpell(Rect *r) {
    Spell *spell;
    int TTL;
    Point p;

    TTL = 0;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL++ >= TTL_MAX)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable());

    if (TTL >= TTL_MAX)
        return;

    SpellFactory sf(this);
    spell = sf.GetRandomSpell();
    spell->SetPosition(p.x, p.y);

    this->items.push_back((Item*) spell);

    if (rand() % 100 <= 10)
        this->AddSpell(r);
}

void 
Level::AddSpellToPlace(Point p, Spell* spell) {
    if(!this->tiles[p.x][p.y].isPassable()) return;
    
    spell->SetPosition(p.x, p.y);

    this->items.push_back((Item*) spell);
}

void
Level::EnemySpawn(Rect *r) {
    EnemyType *et;
    Enemy e;
    int type;
    int TTL;

    const int max_enemies_per_room = 5;

    int enemies_count = rand() % max_enemies_per_room;

    for(int i = 0; i < enemies_count; i++) {
        type = rand() % EnemyTypes::ENEMY_LAST_TYPE;

        et = &EnemyTypeList[type];
        e = Enemy(et);
        TTL = 0;
        do {
            e.pos.x = r->x + rand() % r->w;
            e.pos.y = r->y + rand() % r->h;
            if (TTL++ >= TTL_MAX)
                break;
        } while (GetEnemy(e.pos) || !this->tiles[e.pos.x][e.pos.y].isPassable());

        if (TTL >= TTL_MAX)
            return;

        this->enemies.push_back(e);
    }
}

void
Level::AddBomb(Rect *r) {
    int TTL;
    Point p;

    TTL = 0;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL++ >= TTL_MAX)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable() || this->GetBomb(p));

    if (TTL >= TTL_MAX)
        return;

    Bomb* bomb = new Bomb(p, 10, ',', Colour::COL_GREYWHITE);
    this->bombs.push_back(bomb);

    if (rand() % 100 <= 10)
        this->AddBomb(r);
}

void
Level::AddSpawner(Rect* r) {
    int TTL;
    Point p;

    TTL = 0;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL++ >= TTL_MAX)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable());

    if (TTL >= TTL_MAX)
        return;

    Tile* tile = &this->tiles[p.x][p.y];
    tile->setTileType(SPAWNER_CHAR);

    EnemyTypes type = (EnemyTypes)(rand() % EnemyTypes::ENEMY_LAST_TYPE);

    Spawner* sp = new Spawner(type, *r, this, 10);
    this->spawners.push_back(sp);
}

void
Level::RemoveEnemy(Enemy *e) {
    for (std::list<Enemy>::iterator it = this->enemies.begin();
    it != this->enemies.end(); it++) {
        if (&*it == e) {
            this->enemies.erase(it);
            return;
        }
    }

    GUI::Alert("Error removing enemy from list. That's bad.");
}

void
Level::GiveEnemiesTurns(Character *c, int n) {
    for (std::vector<Spawner*>::iterator it = this->spawners.begin();
    it != this->spawners.end(); it++) {
        Spawner* sp = *it;
        if(FitsIn(c->pos, sp->rect)) {
            sp->SpawnEnemyWithChance();
        }
    }
    for (std::list<Enemy>::iterator it = this->enemies.begin();
    it != this->enemies.end(); it++) {

        if (!it->isActive && CalculateDistance(it->pos, c->pos) <=
        it->parent_type->sight) {
            if (this->CanSee(it->pos, c->pos))
                it->isActive = true;
        }

        //hardcoded 1000
        if (it->isActive)
            it->mv_energy += 1000 * n;
        if (it->mv_energy >= it->parent_type->mv)
            this->EnemyAdvance(&*it, c);
    }
    GUI::DoRedraw();
}

void
Level::EnemyAdvance(Enemy *e, Character *c) {
    Point target;
    Direction::Type tries[3];
    Tile *t;

    while(e->mv_energy >= e->parent_type->mv) {
        e->mv_energy -= e->parent_type->mv;

        //Target x
        if (e->pos.x > c->pos.x) {
            tries[0] = Direction::WEST;
            tries[1] = Direction::WEST;
        }
        else if (e->pos.x < c->pos.x) {
            tries[0] = Direction::EAST;
            tries[1] = Direction::EAST;
        }
        else {
            tries[0] = Direction::LAST_DIRECTION;
            if (rand() % 2)
                tries[1] = Direction::EAST;
            else
                tries[1] = Direction::WEST;
        }

        //Target y
        if (e->pos.y > c->pos.y) {
            tries[0] = MoveCardinal(Direction::NORTH, tries[0]);
            if (ABS(e->pos.x - c->pos.x) < ABS(e->pos.y - c->pos.y)) {
                tries[2] = tries[1];
                tries[1] = Direction::NORTH;
            }
            else
                tries[2] = Direction::NORTH;
        }
        else if (e->pos.y < c->pos.y) {
            tries[0] = MoveCardinal(Direction::SOUTH, tries[0]);
            if (ABS(e->pos.x - c->pos.x) < ABS(e->pos.y - c->pos.y)) {
                tries[2] = tries[1];
                tries[1] = Direction::SOUTH;
            }
            else
                tries[2] = Direction::SOUTH;
        }
        else
            tries[2] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );

        if (tries[0] == Direction::LAST_DIRECTION)
            tries[0] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );
        if (tries[0] == tries[1])
            tries[1] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );

        for (int i = 0; i < 3; ++i) {
            target = GetRelativePoint(tries[i], e->pos);

            if (target == c->pos) {
                e->Attack(c);
                return;
            }

            if (this->GetEnemy(target)) // Can't move into an occupied space
                continue;

            t = &this->tiles[target.x][target.y];

            if (t->getTileType() == FLOOR_CHAR
            || t->getTileType() == OPEN_DOOR_CHAR) {
                e->pos = target;
                return;
            }
        }
    }
}

Enemy*
Level::GetEnemy(const Point& p) {
    for (std::list<Enemy>::iterator it = this->enemies.begin();
            it != this->enemies.end(); it++) {
        if (it->pos == p)
            return &*it;
    }
    return NULL;
}

Item *
Level::GetItem(const Point& p) {
    for (std::list<Item*>::iterator it = this->items.begin();
            it != this->items.end(); it++) {
        if ((*it)->pos == p)
            return &**it;
    }
    return NULL;
}

Bomb*
Level::GetBomb(const Point& p) {
    for (std::vector<Bomb*>::iterator it = this->bombs.begin();
            it != this->bombs.end(); it++) {
        if ((*it)->p == p)
            return *it;
    }
    return NULL;
}

void 
Level::RemoveBomb(Bomb* bomb) {
    for (std::vector<Bomb*>::iterator it = this->bombs.begin();
    it != this->bombs.end(); it++) {
        if (*it == bomb) {
            this->bombs.erase(it);
            return;
        }
    }
}

void
Level::AddPillars(Rect *r) {
    Point p;

    p.x = r->x + 1;
    p.y = r->y + 1;
    this->tiles[p.x][p.y].setTileType(PILLAR_CHAR);
    p.y = r->y + r->h - 2;
    this->tiles[p.x][p.y].setTileType(PILLAR_CHAR);
    p.x = r->x + r->w - 2;
    this->tiles[p.x][p.y].setTileType(PILLAR_CHAR);
    p.y = r->y + 1;
    this->tiles[p.x][p.y].setTileType(PILLAR_CHAR);
}

void
Level::AddEquippable(Rect *r) {
    Equippable *equippable;
    int TTL;
    Point p;
    
    TTL = 0;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL++ >= TTL_MAX)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable());

    if (TTL >= TTL_MAX) 
        return;

    equippable = EquippableFactory().GetRandom();
    equippable->SetPosition(p.x, p.y);

    this->items.push_back((Item*) equippable);

    if (rand() % 100 <= 10)
        this->AddEquippable(r);
}

void
Level::RevealAll(void) {
    for (int e = 0; e < MAP_H; ++e) {
        for (int i = 0; i < MAP_W; ++i) {
            this->tiles[i][e].isVisible = true;
        }
    }
}

bool 
Level::CanSee(Point p1, Point p2) {
    Direction::Type d;
    Tile *t;

    while (p1.x != p2.x || p1.y != p2.y) {
        t = &this->tiles[p1.x][p1.y];
        if (!t->isSeeThrough())
            return false;
        d = DirectionFromAToB(p1, p2);    
        p1 = GetRelativePoint(d, p1);
    }
    return true;
}

void
Level::ClearObstacles(void) {
    for (int e = 0; e < MAP_H; ++e)
        for (int i = 0; i < MAP_W; ++i)
            this->tiles[i][e].setTileType(FLOOR_CHAR);
}

Level::~Level(void) {
    Item *item;
    for (std::list<Item*>::iterator it = this->items.begin();
    it != this->items.end(); it++) {
        item = &**it;
        delete item;
    }
}

void
Level::DoAttack(Enemy *e, int dam) {
    std::stringstream ss;

    ss << e->Description() << " got " << dam << " damage";

    e->isActive = true;
    e->TakeDamage(dam);

    if (!e->isAlive()) {
        ss << " and dies!";
        this->RemoveEnemy(e);
        GUI::DoRedraw();
    }
    
    GUI::AddMessage(ss.str());
}

void 
Level::AddMutBlock(Rect* r) {
    int TTL;
    Point p;

    TTL = 0;
    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
        if (TTL++ >= TTL_MAX)
            break;
    } while (this->GetItem(p) || !this->tiles[p.x][p.y].isPassable());

    if (TTL >= TTL_MAX)
        return;

    Tile* tile = &this->tiles[p.x][p.y];
    tile->setTileType(MUT_BLOCK_CHAR);

    if((rand() % 100) < 50)
        this->AddMutBlock(r);
}

void Level::AddProjectile(Projectile* proj) {
    projs.push_back(proj);
}

void Level::MoveProjectiles() {
    for(std::vector<Projectile*>::iterator it = projs.begin(); 
        it != projs.end();) {
        Projectile* proj = *it;
        Point new_p_t = GetRelativePoint(proj->dir, *proj->p);
        Point* new_p = new Point();
        new_p->x = new_p_t.x;
        new_p->y = new_p_t.y;

        Enemy* enemy = this->GetEnemy(*new_p);

        expl_type e_type = expl_type::LAST_TYPE;

        if(enemy != NULL) {
            DoAttack(enemy, proj->dam);
            e_type = expl_type::ENEMY;
        }

        Tile* t = &tiles[new_p->x][new_p->y];
        if(!t->isPassable()) {
            e_type = expl_type::WALL;
            if(t->isMut()) {
                t->setTileType(FLOOR_CHAR);
                AddSpellToPlace(*new_p, new MutableBlock(this));
            }
        }

        if(proj->lifetime != -1) {
            if(proj->cur_lifetime <= 0) {
                e_type = expl_type::LIFETIME_END;
            }
            else {
                proj->cur_lifetime--;
            }
        }
        
        if(e_type == LAST_TYPE) {
            proj->p = new_p;
            it++;
        }
        else {
            proj->Explose(e_type, new Point(*proj->p));
            it = projs.erase(it);
        }
    }
}

void
Level::PlaceMutableBlock(Point p) {
    bool canPlaceBlock = 
        !GetEnemy(p) && this->tiles[p.x][p.y].isPassable() && !GetBomb(p);

    if(canPlaceBlock) {
        Tile* tile = &this->tiles[p.x][p.y];
        tile->setTileType(MUT_BLOCK_CHAR);
    }
}

Direction::Type
Level::GetInputDir() {
    GUI::SetStatus("Select direction...");
    GameMode::Type old_gm = GUI::g->game_mode;
    GUI::g->game_mode = GameMode::MAP_WALK;
    GUI::DoRedraw();
    char c = getch();
    GUI::g->game_mode = old_gm;
    if (c == '8' || c == 'w')
        return Direction::NORTH;
    else if (c == '2' || c == 'x')
        return Direction::SOUTH;
    else if (c == '6' || c == 'd')
        return Direction::EAST;
    else if (c == '4' || c == 'a')
        return Direction::WEST;
    else if (c == '7' || c == 'q')
        return Direction::NW;
    else if (c == '9' || c == 'e')
        return Direction::NE;
    else if (c == '1' || c == 'z')
        return Direction::SW;
    else if (c == '3' || c == 'c')
        return Direction::SE;
    else {
        return Direction::LAST_DIRECTION;
    }
}