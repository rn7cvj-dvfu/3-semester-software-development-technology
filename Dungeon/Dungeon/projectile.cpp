#include "projectile.hpp"

Projectile::Projectile(char symbol, Direction::Type dir, Point* p, Colour c, int dam, int lifetime, std::function<void(expl_type, Point*)>* e_handler) : 
    symbol(symbol), dir(dir), p(p), c(c), dam(dam), e_handler(e_handler), lifetime(lifetime), cur_lifetime(lifetime) { }

void Projectile::Explose(expl_type type, Point* p) const {
    if(e_handler != nullptr) (*e_handler)(type, p);
}
