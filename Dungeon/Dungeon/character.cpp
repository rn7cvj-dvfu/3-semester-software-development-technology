#include "character.hpp"
#include "gui.hpp"
#include "equippable.hpp"
#include <sstream>
#include <cstdlib>

Character::Character(std::string inName) {
    this->name = inName;
    this->symbol = '@';
    this->colour = COL_GREEN;
    this->gold = 0;

    this->monsters_killed = 0;
    this->total_gold_collected = 0;

    this->baseHp  = 100;
    this->baseAttack = 10;
    this->baseDef = 0;
    this->sight = 6;

    this->curHp = this->baseHp;
    this->curAttack = this->baseAttack;
    this->curDef = this->baseDef;

    for (int i = 0; i < this->max_equip_len; i++) {
        this->equip.push_back(nullptr);
    }

    this->RecalcEffective();
}

void
Character::MoveTo(Point p) {
    this->pos = p;
}

void
Character::ItemToInventory(Item *i) {
    Inventory.push_front(i);
}

void
Character::UseSpell(int n) {
    Item *item = NULL;
    Spell *spell;
    std::list<Item*>::iterator it;

    if (n < 0)
        return;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::SPELL) {
            if (n == 0)
                break;
            --n;
        }
    }

    if (n != 0 || !item) {
        GUI::Alert("Something went wrong trying to find the selected spell...");
        return;
    }

    this->Inventory.erase(it);
    spell = (Spell*) item;
    spell->ApplyEffects(this);

    delete item;
}

void
Character::Heal(int n) {
    std::stringstream ss;

    int lastHp = this->curHp;
    this->curHp = std::min(this->baseHp, this->curHp + n);
    ss << "Recovered " << this->curHp - lastHp << " hp";

    GUI::AddMessage(ss.str());
}

bool
Character::isAlive(void) {
    return this->curHp > 0;
}

void
Character::RecalcEffective(void) {
    this->curAttack = this->baseAttack;
    this->curDef = this->baseDef;
    for (int i = 0; i < this->max_equip_len; i++) {
        if (this->equip[i] == nullptr) continue;
        this->curAttack = this->curAttack + this->equip[i]->AttackMod();
        this->curDef = this->curDef + this->equip[i]->DefMode();
    }
}

int
Character::TakeDamage(int n) {
    int attResist = 0;
    if(this->curDef != 0) attResist = 0.1 * (this->curDef + (rand() % this->curDef));
    int dam = n - attResist;
    int normalized_dam = dam <= 0 ? 1 : dam;
    this->curHp -= normalized_dam;
    return normalized_dam;
}

void
Character::Unequip(int loc) {
    if (!this->equip[loc])
        return;

    this->ItemToInventory(this->equip[loc]);
    this->equip[loc] = NULL;

    this->RecalcEffective();
    cur_equip_len--;
    return;
}

bool
Character::Equip(Equippable *e) {
    std::stringstream ss;

    if (!this->isEquipSlotFree()) {
        ss << "Unequip " << this->equip[this->max_equip_len - 1]->GetName();
        ss << " ?";

        if (!GUI::BinaryChoice(ss.str(), 'y', 'n')) return false;
        this->Unequip(this->max_equip_len - 1);
    }

    if (!this->isEquipSlotFree()) return false;

    this->equip[cur_equip_len] = e;
    this->RecalcEffective();
    cur_equip_len++;
    return true;
}

bool
Character::isEquipSlotFree() {
    return cur_equip_len < this->max_equip_len;
}

Spell* 
Character::SpellFromInventory(int n) {
    std::list<Item*>::iterator it;
    Item* item;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); it++) {
        item = &**it;
        if (item->type != Item::SPELL)
            continue;
        if (n == 0)
            return (Spell*) item;
        --n;
    }

    return NULL;
}

Spell* 
Character::SpellFromList(int n) {
    std::list<Spell*>::iterator it;

    for (it = this->SpellList.begin(); it != this->SpellList.end(); it++) {
        if (n == 0)
            return &**it;
        --n;
    }

    return NULL;
}

void
Character::GiveGold(unsigned int n) {
    this->gold += n;
    this->total_gold_collected += n;
}

int
Character::getGold(void) {
    return this->gold;
}

void
Character::AddSpell(Spell* sp) {
    for (std::list<Spell*>::iterator it = this->SpellList.begin(); it !=
    this->SpellList.end(); it++) {
        if (&**it == sp)
            return;
    }
    this->SpellList.push_back(sp);
}

int
Character::getCurHP(void) {
    return this->curHp;
}

Character::~Character(void) {
    Item *item;
    for (std::list<Item*>::iterator it = this->Inventory.begin();
        it != this->Inventory.end(); it++) {
        item = &**it;
        delete item;
    }

    for (int i = 0; i < this->max_equip_len; i++) {
        if (this->equip[i] != nullptr) delete this->equip[i];
    }
}

std::string
Character::GiveRandomName(void) {
    const char Vowels[]  = { 'a', 'e', 'i', 'o', 'u' };
    const char Conson[] = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 
        'n', 'p', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
    char ch;

    this->name = "";

    int j = rand() % 2;
    while (this->name.size() < 7) {
        if (j) {
            ch = toupper(Vowels[(rand() % 5)]);
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            ch = Vowels[(rand() % 5)];
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            ch = Vowels[(rand() % 5)];
            this->name.append(1, ch);
            break;
        }
        else {
            /* this->name starts with a consonate. */
            ch = toupper(Conson[(rand() % 19)]);
            this->name.append(1, ch);
            ch = Vowels[(rand() % 5)];
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            ch = Vowels[(rand() % 5)];
            this->name.append(1, ch);
            ch = Conson[(rand() % 19)];
            this->name.append(1, ch);
            break;
        }
    }

    return this->name;
}
