#ifndef MONSTER_H
#define MONSTER_H

#include "character.h"

#define DEFAULT_MONSTER_NAME "CuboDaMorte"
#define DEFAULT_MONSTER_MESH "./models/teste.md2"
#define DEFAULT_EXPERIENCE_GIVEN 1000
#define DEFAULT_MONSTER_HP 50
#define DEFAULT_MONSTER_RANGE 10
#define DEFAULT_MONSTER_LEVEL 1
#define DEFAULT_MONSTER_DAMAGE 10

class Monster : public Character {
    private:
        int experienceGiven_;
        int range_;

    protected:

    public:
        int getExperienceGiven()  { return experienceGiven_; }
        int getRange() { return range_; };

        virtual void levelUp();
        virtual float getDamage() { return DEFAULT_MONSTER_DAMAGE; };
        virtual bool walk(vector3df delta);
        virtual void refresh() {}

        Monster(ISceneNode * parent,
                ISceneManager * manager,
                std::string name = DEFAULT_MONSTER_NAME,
                char * modelPath = DEFAULT_MONSTER_MESH,
                int experienceGiven = DEFAULT_EXPERIENCE_GIVEN,
                int maxHP = DEFAULT_MONSTER_HP,
                int range = DEFAULT_MONSTER_RANGE,
                int level = DEFAULT_MONSTER_LEVEL,
                float moveSpeed = DEFAULT_CHARACTER_MOVESPEED);

        ~Monster();
};

#endif // MONSTER_H
