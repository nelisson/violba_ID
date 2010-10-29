#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"

class Weapon : public Item {
    private:
        int minDamage;
        int maxDamage;
        int range;
        int attackSpeed;

    protected:

    public:
        Weapon(int level, string name);
        virtual ~Weapon();
};

#endif // WEAPON_H
