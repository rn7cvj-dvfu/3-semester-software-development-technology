#ifndef BOMB_H
#define BOMB_H

#include "geometry.hpp"

class Bomb {
public:
    Point p;
    int dmg;
    char symbol;
    Colour colour;

    Bomb(Point p, int dmg, char symbol, Colour colour);

};

#endif