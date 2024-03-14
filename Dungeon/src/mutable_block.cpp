#include "mutable_block.hpp"

MutableBlock::MutableBlock(Level* level) : Spell(level) {
    this->type = Item::SPELL;
    this->colour = COL_YELLOW;
    this->symbol = '=';
    this->_name = "Block";
}

void MutableBlock::ApplyEffects(Character* c) {
    Direction::Type dir = _cur_level->GetInputDir();
    if(dir == Direction::Type::LAST_DIRECTION) {
        GUI::SetStatus("Aborted");
    }
    Point p(GetRelativePoint(dir, c->pos));
    _cur_level->PlaceMutableBlock(p);
}
