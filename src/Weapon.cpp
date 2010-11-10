#include "Weapon.h"

Weapon::Weapon(string name,
               int level,
               float minDamage,
               float maxDamage,
               int range,
               float attackSpeed,
               float attackAngle)
    : Item(name, level) {

    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    range_ = range;
    attackSpeed_ = attackSpeed;
    attackAngle_ = attackAngle;
}

Weapon::~Weapon() {}

