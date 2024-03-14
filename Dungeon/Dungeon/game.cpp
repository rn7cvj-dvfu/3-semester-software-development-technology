#include <cstdlib>
#include <sstream>
#include <list>
#include "game.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "treasure.hpp"
#include "scrollable_menu.hpp"
#include "healing_potion.hpp"
#include "scroll_fire.hpp"
#include "bomb.hpp"
#include "mutable_block.hpp"

#define KEYS_QUIT               27
#define KEYS_INVENTORY          'i'
#define KEYS_BOW                'l'
#define KEYS_SHOTGUN            'm'
#define KEYS_ESCAPE             '`'
#define KEYS_SPELL_SELECT       'p'
#define KEYS_GEAR_SELECT        'g'

Game::Game(Character *c) {
    this->character = c;
    this->cur_level = new Level(NULL);
    this->character->MoveTo(this->cur_level->start_point);
    GUI::CentreCam(character->pos);

    this->cur_level->RevealSight(this->character->pos, this->character->sight);
    GUI::SetStatus("Welcome to The Dungeon!");

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;

    this->SpellSelectMenu = new ScrollableMenu("Spells");
    this->EquipSelectMenu = new ScrollableMenu("Equipment");

    this->character->ItemToInventory((Item*) new HealingPotion(this->cur_level, HealingPotion::MINOR));
    this->character->ItemToInventory((Item*) new HealingPotion(this->cur_level, HealingPotion::MINOR));
    this->character->ItemToInventory((Item*) new ScrollFire(this->cur_level));
    this->character->ItemToInventory((Item*) new MutableBlock(this->cur_level));

    this->need_hard_redraw = false;

    GUI::AttachTo(this);
}

bool
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = GameMode::LAST_MODE;
    GUI::DrawLevel(this->cur_level);

    while (this->running) {
        c = getch();

        if (c == KEY_F(12))
            GUI::QuitDialogue(this);
        else if (c == KEYS_QUIT)
            GUI::QuitDialogue(this);
        else if (c == KEY_F(4))
            GUI::HardRedraw();
        else if (c == KEYS_BOW)
            new_gamemode = GameMode::SELECT_BOW_DIR;
        else if (c == KEYS_SHOTGUN)
            new_gamemode = GameMode::SELECT_SHOTGUN_DIR;
        else if (c == KEYS_INVENTORY)
            new_gamemode = GameMode::INVENTORY_SCREEN;
        else if (c == KEYS_SPELL_SELECT)
            new_gamemode = GameMode::SPELL_SELECT;
        else if (c == KEYS_ESCAPE)
            new_gamemode = GameMode::MAP_WALK;
        else if (c == KEYS_GEAR_SELECT)
            new_gamemode = GameMode::GEAR_SELECT;
        else
            new_gamemode = this->HandleInput(c);

        if (new_gamemode == this->game_mode)
            new_gamemode = GameMode::MAP_WALK;

        if (new_gamemode != GameMode::LAST_MODE)
            this->SwitchGameMode(new_gamemode);

        if (this->game_mode == GameMode::MAP_WALK)
            GUI::ProcessMessages(this);

        if (!this->character->isAlive()) {
            running = false;
            GUI::Alert("You have died!");
        }

        if (this->need_hard_redraw) {
            this->need_hard_redraw = false;
            GUI::HardRedraw();
        }
    }

    GUI::ShowTombstone(this);
    getch();
    GUI::FancyClear();
    GUI::ShowSplash();
    return GUI::BinaryChoice("Do you wish to play again?", 'y', 'n');
}

void
Game::SwitchGameMode(GameMode::Type gmt) {
    if(this->game_mode == gmt)
        return;

    this->game_mode = gmt;

    switch (gmt) {
        case GameMode::MAP_WALK:
            GUI::CentreCam(this->character->pos);
            GUI::DoRedraw();
            GUI::CharacterStatusLine(this->character);
            break;
        case GameMode::INVENTORY_SCREEN:
            GUI::ShowInventoryScreen(this->character);
            break;
        case GameMode::SPELL_SELECT:
            GUI::FancyClear();
            this->SpellSelectMenu->Reset();
            this->RepopulateSpellMenu();
            this->SpellSelectMenu->Show();
            break;
        case GameMode::GEAR_SELECT:
            GUI::FancyClear();
            this->EquipSelectMenu->Reset();
            this->RepopulateEquipMenu();
            this->EquipSelectMenu->Show();
            break;
        case GameMode::SELECT_BOW_DIR:
        case GameMode::SELECT_SHOTGUN_DIR:
            GUI::SetStatus("Select dir of projectile.");
            break;
        case GameMode::LAST_MODE:
        default:
            GUI::Alert("Error: Asked to switch to an unknown game mode");
            break;
    }
}

GameMode::Type
Game::HandleInput(int c) {
    switch (this->game_mode) {
        case GameMode::MAP_WALK:
            if (c == KEY_UP)
                GUI::MoveCamera(Direction::NORTH);
            else if (c == KEY_DOWN)
                GUI::MoveCamera(Direction::SOUTH);
            else if (c == KEY_RIGHT)
                GUI::MoveCamera(Direction::EAST);
            else if (c == KEY_LEFT)
                GUI::MoveCamera(Direction::WEST);
            else if (c == '8' || c == 'w')
                this->MoveCharacter(Direction::NORTH);
            else if (c == '2' || c == 'x')
                this->MoveCharacter(Direction::SOUTH);
            else if (c == '6' || c == 'd')
                this->MoveCharacter(Direction::EAST);
            else if (c == '4' || c == 'a')
                this->MoveCharacter(Direction::WEST);
            else if (c == '7' || c == 'q')
                this->MoveCharacter(Direction::NW);
            else if (c == '9' || c == 'e')
                this->MoveCharacter(Direction::NE);
            else if (c == '1' || c == 'z')
                this->MoveCharacter(Direction::SW);
            else if (c == '3' || c == 'c')
                this->MoveCharacter(Direction::SE);
            else if (c == 's' || c == '5')
                this->DoWait();
            else if (c == ',')
                this->DoPickup();
            else if (c == '\n')
                GUI::RedrawStatus();
            break;
        case GameMode::SPELL_SELECT:
            if (c == KEY_UP)
                this->SpellSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->SpellSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->SpellSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->SpellSelectMenu->PageDown();
            else if (c == 'v') {
                c = this->SpellSelectMenu->Selection();
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                this->character->UseSpell(this->SpellSelectMenu->Selection());
                this->cur_level->MoveProjectiles();
                this->cur_level->GiveEnemiesTurns(this->character, 1);
                GUI::DoRedraw();
                return GameMode::MAP_WALK;
            }
            break;
        case GameMode::GEAR_SELECT:
            if (c == KEY_UP)
                this->EquipSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->EquipSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->EquipSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->EquipSelectMenu->PageDown();
            else if (c == 'v') {
                c = this->EquipSelectMenu->Selection();
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                this->HandleEquipSelection(this->EquipSelectMenu->Selection());
                this->cur_level->MoveProjectiles();
                this->cur_level->GiveEnemiesTurns(this->character, 1);
                GUI::DoRedraw();
            }
            break;
        case GameMode::SELECT_BOW_DIR: {
            Direction::Type type = Direction::Type::LAST_DIRECTION;
            if (c == '8' || c == 'w')
                type = Direction::NORTH;
            else if (c == '2' || c == 'x')
                type = Direction::SOUTH;
            else if (c == '6' || c == 'd')
                type = Direction::EAST;
            else if (c == '4' || c == 'a')
                type = Direction::WEST;
            else if (c == '7' || c == 'q')
                type = Direction::NW;
            else if (c == '9' || c == 'e')
                type = Direction::NE;
            else if (c == '1' || c == 'z')
                type = Direction::SW;
            else if (c == '3' || c == 'c')
                type = Direction::SE;
            if(type == Direction::Type::LAST_DIRECTION) {
                GUI::AddMessage("Return to walk mode.");
                return GameMode::MAP_WALK;
            }
            
            DoBow(type);
            return GameMode::MAP_WALK;
            }
            break;
        case GameMode::SELECT_SHOTGUN_DIR: {
            Direction::Type type = Direction::Type::LAST_DIRECTION;
            if (c == '8' || c == 'w')
                type = Direction::SOUTH;
            else if (c == '2' || c == 'x')
                type = Direction::NORTH;
            else if (c == '6' || c == 'd')
                type = Direction::WEST;
            else if (c == '4' || c == 'a')
                type = Direction::EAST;

            if(type == Direction::Type::LAST_DIRECTION) {
                GUI::AddMessage("Return to walk mode.");
                return GameMode::MAP_WALK;
            }

            DoShotgun(type);
            return GameMode::MAP_WALK;
            }
            break;
        default:
            break;
    }
    return GameMode::LAST_MODE;
}

void
Game::MoveCharacter(Direction::Type d) {
    Point target;
    Character *c;
    Tile *t;
    std::list<Enemy>::iterator it;

    c = this->character;
    target = c->pos;

    if (d == Direction::NORTH)
        target.y--;
    else if (d == Direction::SOUTH)
        target.y++;
    else if (d == Direction::EAST)
        target.x++;
    else if (d == Direction::WEST)
        target.x--;
    else if (d == Direction::NW) {
        target.x--;
        target.y--;
    }
    else if (d == Direction::NE) {
        target.x++;
        target.y--;
    }
    else if (d == Direction::SW) {
        target.x--;
        target.y++;
    }
    else {
        target.x++;
        target.y++;
    }

    t = &this->cur_level->tiles[target.x][target.y];

    for (it = this->cur_level->enemies.begin();
            it != this->cur_level->enemies.end(); it++) {
        if (target == it->pos) {
            this->DoAttack(this->character, &*it);            
            break;
        }
    }

    if (it != this->cur_level->enemies.end()) {              
    }
    else if (t->getTileType() == FLOOR_CHAR || t->getTileType() == OPEN_DOOR_CHAR) { 
            c->MoveTo(target);
            this->cur_level->RevealSight(c->pos, c->sight);
            GUI::CentreCam(c->pos);
            this->ReportStandingOn();
            Bomb* bomb = this->cur_level->GetBomb(c->pos);
            if(bomb) {
                c->TakeDamage(bomb->dmg);
                for(int i = 0; i < Direction::Type::LAST_DIRECTION; i++) {
                    this->cur_level->AddProjectile(
                        new Projectile('*', (Direction::Type)i, new Point(c->pos), Colour::COL_RED, bomb->dmg, 3)
                    );
                }                
                this->cur_level->RemoveBomb(bomb);
            } 
    }
    else if (t->getTileType() == CLOSED_DOOR_CHAR) {          
        if (GUI::BinaryChoice("This door is closed. Open?", 'y', 'n')) {
            t->setTileType(OPEN_DOOR_CHAR);
            this->cur_level->RevealSight(c->pos, c->sight);
        }
    }
    else                                                      
        return;

    GUI::DoRedraw();
    this->cur_level->MoveProjectiles();
    this->cur_level->GiveEnemiesTurns(c, 1);
    GUI::DoRedraw();
}

void
Game::DoPickup(void) {
    Item *item;
    std::stringstream ss;
    std::list<Item*>::iterator it;

    for (it = this->cur_level->items.begin(), item = NULL;
            it != this->cur_level->items.end(); ++it) {
        if ((*it)->pos == this->character->pos) {
            item = &**it;
            break;
        }
    }

    if (!item) {
        GUI::AddMessage("Nothing to pick up.");
    }
    else {
        ss << "You pick up " << item->GetName();;
        if (item->type == Item::TREASURE_T) {
            this->character->GiveGold(((Treasure*)item)->Quantity());
            delete (Treasure*) item;
        }
        else
            this->character->ItemToInventory(item);
        GUI::AddMessage(ss.str());
        this->cur_level->items.erase(it);
        this->cur_level->MoveProjectiles();
        this->cur_level->GiveEnemiesTurns(this->character, 1);
    }
}

void
Game::HandleResize(int signal) {
    refresh();
    GUI::DoRedraw();
}

void
Game::DoAttack(Character *c, Enemy *e) {
    std::stringstream ss;

    ss << c->name << " attacks "  << e->Description() << " and";

    e->isActive = true;
    e->TakeDamage(c->curAttack);
    if (e->isAlive()) {
        ss << "cause " << c->curAttack << " damage";
    }
    else {
        c->monsters_killed++;
        ss << " and kill him!";
        this->cur_level->RemoveEnemy(e);
        GUI::DoRedraw(); 
    }
    
    GUI::AddMessage(ss.str());
}

void
Game::DoWait(void) {
    this->cur_level->MoveProjectiles();
    this->cur_level->GiveEnemiesTurns(this->character, 1);
    GUI::DoRedraw();
}

void
Game::RepopulateSpellMenu(void) {
    Character *c;
    Item* item;

    c = this->character;

    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::SPELL)
            this->SpellSelectMenu->AddItem(item->GetName());
    }
}

void
Game::RepopulateEquipMenu(void) {
    Character *c;
    Item* item;
    std::stringstream ss;

    c = this->character;

    for (int i = 0; i < c->max_equip_len; ++i) {
        if (c->equip[i] != nullptr) {
            ss.str("");
            ss << "E:" << c->equip[i]->GetName();
            this->EquipSelectMenu->AddItem(ss.str());
        }
    }

    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::EQUIPPABLE)
            this->EquipSelectMenu->AddItem(item->GetName());
    }
}

void
Game::HandleEquipSelection(int n) {
    std::list<Item*>::iterator it;
    Character *c;
    Equippable *e;
    Item *item;

    c = this->character;
    n++;

    for (int i = 0; i < c->max_equip_len; ++i) {
        if (c->equip[i])
            --n;
        if (n == 0) {
            c->Unequip(i);
            if (c->isEquipSlotFree()) {
                this->EquipSelectMenu->Reset();
                this->RepopulateEquipMenu();
            }
            return;
        }
    }

    for (it = c->Inventory.begin(); it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::EQUIPPABLE)
            --n;
        if (n == 0) {
            e = (Equippable*) item;
            break;
        }
    }

    if (n != 0) {
        GUI::Alert("Error syncing up equip/deequip selection!");
        return;
    }

    if ( !c->Equip(e) )
        return;

    c->Inventory.erase(it);
    this->EquipSelectMenu->Reset();
    this->RepopulateEquipMenu();
}

void
Game::ReportStandingOn(void) {
    Item *item;

    item = this->cur_level->GetItem(this->character->pos);
    if (!item)
        return;

    GUI::AddMessage("STANDING ON: " + item->GetName());
}

void Game::DoBow(Direction::Type type) {
    Projectile* ptr = new Projectile('+', type, &character->pos, Colour::COL_PURPLE, 10);
    if(character->pm != nullptr) {
        if(character->pm->IsActive()) {
            character->pm->Apply(ptr);
            character->pm->NextTurn();
        }
    }
    this->cur_level->AddProjectile(ptr);
}

void Game::DoShotgun(Direction::Type type) {
    for(int i = 0; i < 4; i++) {
        if(i == type) continue;
        Projectile* ptr = new Projectile('o', (Direction::Type)i, &character->pos, Colour::COL_RED, 5);
        this->cur_level->AddProjectile(ptr);
    }
}

Game::~Game(void) {
    if (this->cur_level)
        delete this->cur_level;
    delete this->SpellSelectMenu;
    delete this->BookSelectMenu;
    delete this->EquipSelectMenu;
}

