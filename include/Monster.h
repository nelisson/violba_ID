#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"
#include "Utils.h"
#include "ctime"
#include <irrklang/irrKlang.h>

#define DEFAULT_MONSTER_NAME "DwarfDaMorte"
#define DEFAULT_MONSTER_MESH "./models/dwarf1.b3d"
#define DEFAULT_MONSTER_MESH2 "./models/dwarf2.b3d"
#define DEFAULT_EXPERIENCE_GIVEN 400
#define DEFAULT_MONSTER_HP 20
#define DEFAULT_MONSTER_RANGE 10
#define DEFAULT_MONSTER_LEVEL 1
#define DEFAULT_MONSTER_MIN_DAMAGE 4
#define DEFAULT_MONSTER_MAX_DAMAGE 8
#define DEFAULT_MONSTER_ATTACK_SPEED 2

#define MONSTER_WALK 1,13
#define MONSTER_RUN 15,25
#define MONSTER_IDLE 74,87
//#define MONSTER_IDLE2 90,110

#define MONSTER_ATTACK 111,125
/*
128-142	Attack 2 - Jump and overhead whack attack
144-160	Attack 3 - 360 spin Back hander
162-180	Attack 4 - 2 Swipes left and right
182-192	Attack 5 - Stab
194-210	Block
*/
#define MONSTER_DIE 211,226
#define MONSTER_DEAD 226,226
/*230-251	Die 2 - Backwards

253-272	Nod YES
274-290	Shake head NO

292-325	Idle 1
327-360	Idle 2
*/

#define DEFAULT_MONSTER_CORPSE_DELAY 5

using namespace irrklang;

class Monster : public Character {

    private:
        int experienceGiven_;
        int range_;
        float attackSpeed_;
        float minDamage_;
        float maxDamage_;

        float corpseDelay_;

        time_t attackStart_;

    protected:

    public:
        float getCorspeDelay() const { return corpseDelay_; }
        void decreaseCorspeDelay(float amount) { corpseDelay_ -= amount; }

        int getExperienceGiven() const { return experienceGiven_; }
        int getRange() const { return range_; };

        virtual void levelUp();
        virtual float getDamage() const { return randomBetween(minDamage_, maxDamage_); };
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node);
        virtual bool walk(vector3df delta);
        virtual void die();
        virtual void refresh(f32 elapsedTime) {}

        bool canAttack();
        void attack();

        Monster(ISceneNode * parent,
                ISceneManager * manager,
                ISoundEngine * soundEngine,
                const char * modelPath,
                const std::string name = DEFAULT_MONSTER_NAME,                
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
