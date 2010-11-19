#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"
#include "Utils.h"
#include <cstdlib>

#define DEFAULT_WEAPON_MINDAMAGE 10
#define DEFAULT_WEAPON_MAXDAMAGE 20
#define DEFAULT_WEAPON_RANGE 10
#define DEFAULT_WEAPON_ATTACKSPEED 30
#define DEFAULT_WEAPON_ATTACKANGLE 60

class Weapon : public Item {
    private:
        int minDamage_;
        int maxDamage_;
        int range_;
        float attackSpeed_;
        float attackAngle_;

    protected:

    public:
        int getMinDamage() { return minDamage_; }
        int getMaxDamage() { return maxDamage_; }
        float getDamage() { return randomBetween(minDamage_, maxDamage_); }
        float getRange() { return range_; }
        float getAttackSpeed() { return attackSpeed_; }
        float getAttackAngle() { return attackAngle_;}

        Weapon(std::string name,
               int level = DEFAULT_ITEM_LEVEL,
               float minDamage = DEFAULT_WEAPON_MINDAMAGE,
               float maxDamage = DEFAULT_WEAPON_MAXDAMAGE,
               int range = DEFAULT_WEAPON_RANGE,
               float attackSpeed = DEFAULT_WEAPON_ATTACKSPEED,
               float attackAngle = DEFAULT_WEAPON_ATTACKANGLE);

        virtual ~Weapon();
};

#endif // WEAPON_H
