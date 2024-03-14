#include "projectile_modifier.hpp"

ProjectileModifier::ProjectileModifier(Level* level, int turns) : cur_level(level), _turns(turns), _turnsLeft(turns) { }

void ProjectileModifier::NewSymbol(char symbol) {
    _isModSymbol = true;
    _modSymbol = symbol;
}

void ProjectileModifier::NewColour(Colour colour) {
    _isModColour = true;
    _modColor = colour;
}

void ProjectileModifier::IncreaseDam(int dam) {
    _isModDam = true;
    _increaseDam = dam;
}

void ProjectileModifier::IncreaseLifetime(int lifetime) {
    _isModLifetime = true;
    _increaseLifetime = lifetime;
}

void ProjectileModifier::NewOnExpl(std::function<void(expl_type, Point*)>* onExpl) {
    _isModOnExpl = true;
    _onExpl = onExpl;
}

void ProjectileModifier::Apply(Projectile* proj) const {
    if(_isModSymbol) proj->symbol = _modSymbol;
    if(_isModColour) proj->c = _modColor;
    if(_isModDam) proj->dam += _increaseDam;
    if(_isModLifetime && proj->lifetime != -1) 
        proj->cur_lifetime = proj->lifetime = proj->lifetime + _increaseLifetime;
    if(_isModOnExpl) proj->e_handler = _onExpl;
}

bool ProjectileModifier::IsActive() const {
    return _turnsLeft > 0;
}

void ProjectileModifier::NextTurn() {
    _turnsLeft--;
}

