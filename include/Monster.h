#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"
#include "Utils.h"
#include "ctime"
#include <irrklang/irrKlang.h>

#define DEFAULT_MONSTER_NAME "DwarfDaMorte"
#define DEFAULT_MONSTER_MESH "./models/dwarf1.b3d"
#define DEFAULT_EXPERIENCE_GIVEN 1000
#define DEFAULT_MONSTER_HP 50
#define DEFAULT_MONSTER_RANGE 10
#define DEFAULT_MONSTER_LEVEL 1
#define DEFAULT_MONSTER_MIN_DAMAGE 5
#define DEFAULT_MONSTER_MAX_DAMAGE 10
#define DEFAULT_MONSTER_ATTACK_SPEED 2

#define MONSTER_WALK 2,14
#define MONSTER_RUN 16,26
#define MONSTER_IDLE 75,88
//#define MONSTER_IDLE2 90,110

#define MONSTER_ATTACK 112,126
/*
128-142	Attack 2 - Jump and overhead whack attack
144-160	Attack 3 - 360 spin Back hander
162-180	Attack 4 - 2 Swipes left and right
182-192	Attack 5 - Stab
194-210	Block
*/
#define MONSTER_DIE 212,227
/*230-251	Die 2 - Backwards

253-272	Nod YES
274-290	Shake head NO

292-325	Idle 1
327-360	Idle 2
*/

using namespace irrklang;

namespace MonsterSound {
    enum Sound {
        SWING,
        DEAD,
    };
}

class Monster : public Character {

    private:
        int experienceGiven_;
        int range_;
        float attackSpeed_;
        float minDamage_;
        float maxDamage_;

        time_t attackStart_;

    protected:

    public:
        int getExperienceGiven() const { return experienceGiven_; }
        int getRange() const { return range_; };

        virtual void levelUp();
        virtual float getDamage() const { return randomBetween(minDamage_, maxDamage_); };
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node);
        virtual bool walk(vector3df delta);
        virtual void die();
        virtual void refresh() {}

        bool canAttack();
        void attack();

        Monster(ISceneNode * parent,
                ISceneManager * manager,
                ISoundEngine * soundEngine,
                const std::string name = DEFAULT_MONSTER_NAME,
                const char * modelPath = DEFAULT_MONSTER_MESH,
                int experienceGiven = DEFAULT_EXPERIENCE_GIVEN,
                int maxHP = DEFAULT_MONSTER_HP,
                int range = DEFAULT_MONSTER_RANGE,
                int level = DEFAULT_MONSTER_LEVEL,
                float moveSpeed = DEFAULT_CHARACTER_MOVESPEED,
                float attackSpeed = DEFAULT_MONSTER_ATTACK_SPEED,
                float minDamage = DEFAULT_MONSTER_MIN_DAMAGE,
                float maxDamage = DEFAULT_MONSTER_MAX_DAMAGE);

        ~Monster();
};

#endif // MONSTER_H
