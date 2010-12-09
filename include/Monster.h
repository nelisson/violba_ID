#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"
#include "Utils.h"
#include "ctime"

#define DEFAULT_MONSTER_NAME "DwarfDaMorte"
#define DEFAULT_MONSTER_MESH "./models/teste.md2"
#define DEFAULT_EXPERIENCE_GIVEN 1000
#define DEFAULT_MONSTER_HP 50
#define DEFAULT_MONSTER_RANGE 10
#define DEFAULT_MONSTER_LEVEL 1
#define DEFAULT_MONSTER_MIN_DAMAGE 0
#define DEFAULT_MONSTER_MAX_DAMAGE 0
#define DEFAULT_MONSTER_ATTACK_SPEED 2

#define ANIM_1 0,12
#define ANIM_2 13,15

class Monster : public Character, public IAnimationEndCallBack {
    private:
        int experienceGiven_;
        int range_;
        float attackSpeed_;
        float minDamage_;
        float maxDamage_;

        time_t attackStart_;

    protected:

    public:
        int getExperienceGiven()  { return experienceGiven_; }
        int getRange() { return range_; };

        virtual void levelUp();
        virtual float getDamage() { return randomBetween(minDamage_, maxDamage_); };
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node);
        virtual bool walk(vector3df delta);
        virtual void die();
        virtual void refresh() {}

        bool canAttack();
        void attack();

        Monster(ISceneNode * parent,
                ISceneManager * manager,
                std::string name = DEFAULT_MONSTER_NAME,
                char * modelPath = DEFAULT_MONSTER_MESH,
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
