#include "Weapon.h"

Weapon::Weapon(int level, string name)
    : Item(level, name) {
}

Weapon::~Weapon()
{
    //dtor
}
