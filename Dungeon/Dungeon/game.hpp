#ifndef GAME_H
#define GAME_H

#include "level.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include "enemy.hpp"
#include "scrollable_menu.hpp"
#include <string>

class Level;

namespace GameMode {
    typedef enum {
        MAP_WALK,
        INVENTORY_SCREEN,
        SPELL_SELECT,
        GEAR_SELECT,
        SELECT_BOW_DIR,
        SELECT_SHOTGUN_DIR,
        LAST_MODE
    } Type;
}

class Game {
    public:
    Game(Character *c);
    ~Game();
    bool Run();
    GameMode::Type HandleInput(int c);
    void SwitchGameMode(GameMode::Type gmt);
    void MoveCharacter(Direction::Type d);
    void DrawLookTarget();
    void MoveLookTarget(Direction::Type d);
    void DoPickup();
    void HandleResize(int signal);
    void DoAttack(Character *c, Enemy *e);
    void DoWait();
    void RepopulateSpellMenu();
    void RepopulateEquipMenu();
    void HandleEquipSelection(int n);
    void ReportStandingOn();

    Direction::Type GetInputDir();

    void DoBow(Direction::Type type);
    void DoShotgun(Direction::Type type);

    Level*              cur_level;
    Character*          character;
    bool                running;
    bool                need_hard_redraw;
    GameMode::Type      game_mode;
    Point               target;
    ScrollableMenu      *SpellSelectMenu;
    ScrollableMenu      *BookSelectMenu;
    ScrollableMenu      *EquipSelectMenu;
};

#endif
