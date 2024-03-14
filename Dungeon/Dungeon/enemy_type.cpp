#include "enemy_type.hpp"

EnemyType::EnemyType(
    char symbol, 
    Colour colour, 
    int hp, 
    int mv, 
    int sight, 
    int attack,
    std::string name) 
    :
    symbol(symbol),
    colour(colour),
    hp(hp),
    mv(mv),
    sight(sight),
    attack(attack),
    name(name) 
    { }