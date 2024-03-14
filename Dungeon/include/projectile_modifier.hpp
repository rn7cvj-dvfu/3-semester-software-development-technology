#ifndef PROJECTILEMODIFIER_H
#define PROJECTILEMODIFIER_H

#include <functional>
#include "projectile.hpp"
#include "level.hpp"

class ProjectileModifier {
private:
    int _turnsLeft;
    int _turns;

    bool _isModSymbol = false;
    char _modSymbol;

    bool _isModColour = false;
    Colour _modColor;
    
    bool _isModDam = false;
    int _increaseDam;

    bool _isModLifetime = false;
    int _increaseLifetime;

    bool _isModOnExpl = false;
    std::function<void(expl_type, Point*)>* _onExpl;

public:
    ProjectileModifier(Level* cur_level, int turns);

    void NewSymbol(char symbol);
    void NewColour(Colour Colour);
    void IncreaseDam(int dam);
    void IncreaseLifetime(int lifetime);
    void NewOnExpl(std::function<void(expl_type, Point*)>* onExpl);

    void Apply(Projectile* proj) const;
    bool IsActive() const;

    void NextTurn();

    Level* cur_level;
};

#endif 