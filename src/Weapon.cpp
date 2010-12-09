#include "Weapon.h"

Weapon::Weapon(ISceneNode * parent,
               ISceneManager * manager,
               std::string name,
               char * modelPath,
               float minDamage,
               float maxDamage,
               int range,
               float attackSpeed,
               float attackAngle)
    : Item(parent,
           manager,
           name,
           modelPath) {

    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    range_ = range;
    attackSpeed_ = attackSpeed;
    attackAngle_ = attackAngle;
}

Weapon::~Weapon() {}

