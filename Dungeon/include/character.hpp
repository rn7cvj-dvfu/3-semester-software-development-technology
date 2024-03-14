#ifndef CHARACTER_H
#define CHARACTER_H

#include "geometry.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "equippable.hpp"
#include "spell.hpp"
#include <string>
#include <list>
#include <vector>
#include "spell.hpp"
#include "projectile_modifier.hpp"

class Enemy;
class Spell;
class ProjectileModifier;

class Character {
public:
    Character(std::string inName);
    ~Character();
    void MoveTo(Point p);
    void ItemToInventory(Item *i);
    void Heal(int n);
    bool isAlive();
    void RecalcEffective();
    int TakeDamage(int n);
    
    void Unequip(int n);
    bool Equip(Equippable *e);
    bool isEquipSlotFree();

    Spell* SpellFromInventory(int n);
    Spell* SpellFromList(int n);
    void AddSpell(Spell *sp);
    void UseSpell(int n);

    void GiveGold(unsigned int n);
    std::string GiveRandomName();

    int getCurHP();
    int getGold();

    char symbol;
    Colour colour;
    Point pos;
    std::string name;
    std::list<Item*> Inventory;
    std::list<Spell*> SpellList;
    std::vector<Equippable*> equip;

    ProjectileModifier* pm = nullptr;

    int baseHp, curHp;
    int baseAttack, curAttack;
    int baseDef, curDef;
    int sight;

    int monsters_killed;
    int total_gold_collected;

    const int max_equip_len = 9;

private:
    int cur_equip_len = 0;
    int gold;
};

#endif
