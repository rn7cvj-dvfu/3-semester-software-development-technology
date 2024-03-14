#include "tile.hpp"

Tile::Tile() {}

Tile::Tile(bool vis, char inC) {
    this->isVisible = vis;
    this->c = inC;
}

bool
Tile::isSeeThrough() {
    if (this->c == WALL_CHAR)
        return false;
    if (this->c == CLOSED_DOOR_CHAR)
        return false;
    if (this->c == PILLAR_CHAR)
        return false;
    if (this->c == MUT_BLOCK_CHAR)
        return false;
    return true;
}

bool
Tile::isPassable() {
    if (this->c == WALL_CHAR)
        return false;
    if (this->c == CLOSED_DOOR_CHAR)
        return false;
    if (this->c == PILLAR_CHAR)
        return false;
    if (this->c == MUT_BLOCK_CHAR)
        return false;
    return true;
}

bool
Tile::isMut() {
    return this->c == MUT_BLOCK_CHAR;
}

char
Tile::getTileType() {
    return this->c;
}

void
Tile::setTileType(char in) {
    this->c = in;
}
