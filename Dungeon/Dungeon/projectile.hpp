#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "geometry.hpp"
#include <functional>

enum expl_type {
    ENEMY,
    WALL,
    LIFETIME_END,
    LAST_TYPE
};

class Projectile {
public:
    char symbol;
    Direction::Type dir;
    Point* p;
    Colour c;
    int dam;
    int lifetime;
    int cur_lifetime;
    std::function<void(expl_type, Point*)>* e_handler;

    Projectile(char symbol, Direction::Type dir, Point* p, Colour c, int dam, int lifetime = -1, std::function<void(expl_type, Point*)>* e_handler = nullptr);
    void Explose(expl_type type, Point* p) const;
};

#endif