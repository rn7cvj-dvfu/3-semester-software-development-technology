#ifndef MUTABLE_BLOCK_H
#define MUTABLE_BLOCK_H

#include "spell.hpp"
#include "game.hpp"
#include "gui.hpp"

class MutableBlock : public Spell {
public:
    MutableBlock(Level* game);

    void ApplyEffects(Character* c);

};

#endif