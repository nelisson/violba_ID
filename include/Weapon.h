#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"
#include "Utils.h"

#include <irrlicht/irrlicht.h>
#include <cstdlib>

#define DEFAULT_WEAPON_MINDAMAGE 10
#define DEFAULT_WEAPON_MAXDAMAGE 20
#define DEFAULT_WEAPON_RANGE 10
#define DEFAULT_WEAPON_ATTACKSPEED 30
#define DEFAULT_WEAPON_ATTACKANGLE 60
#define DEFAULT_WEAPON_MESH_PATH "./models/sword7anim.x"

using namespace irr;
using namespace irr::scene;

class Weapon : public Item {
    private:
        int minDamage_;
        int maxDamage_;
        int range_;
        float attackSpeed_;
        float attackAngle_;

    protected:

    public:
        int getMinDamage() const { return minDamage_; }
        int getMaxDamage() const { return maxDamage_; }
        float getDamage() const { return randomBetween(minDamage_, maxDamage_); }
        float getRange() const { return range_; }
        float getAttackSpeed() const { return attackSpeed_; }
        float getAttackAngle() const { return attackAngle_; }

        Weapon(ISceneNode * parent,
               ISceneManager * manager,
               std::string name,
               const char * modelPath = DEFAULT_WEAPON_MESH_PATH,
               float minDamage = DEFAULT_WEAPON_MINDAMAGE,
               float maxDamage = DEFAULT_WEAPON_MAXDAMAGE,
               int range = DEFAULT_WEAPON_RANGE,
               float attackSpeed = DEFAULT_WEAPON_ATTACKSPEED,
               float attackAngle = DEFAULT_WEAPON_ATTACKANGLE);

        virtual ~Weapon();
};

#endif // WEAPON_H
