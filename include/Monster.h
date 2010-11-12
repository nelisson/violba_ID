#ifndef MONSTER_H
#define MONSTER_H

#include "character.h"

#define DEFAULT_MONSTER_NAME "CuboDaMorte"
#define DEFAULT_MONSTER_MESH "./models/teste.md2"
#define DEFAULT_EXPERIENCE_GIVEN 1000
#define DEFAULT_MONSTER_HP 50
#define DEFAULT_MONSTER_LEVEL 1
#define DEFAULT_MONSTER_DAMAGE 10

class Monster : public Character {
    private:
        int experienceGiven_;

    protected:

    public:
        int getExperienceGiven();
        virtual void levelUp();

        int getRange();
        virtual float getDamage() { return DEFAULT_MONSTER_DAMAGE; };

        Monster(string name = DEFAULT_MONSTER_NAME,
                char * modelPath = DEFAULT_MONSTER_MESH,
                int experienceGiven = DEFAULT_EXPERIENCE_GIVEN,
                int maxHP = DEFAULT_MONSTER_HP,
                int level = DEFAULT_MONSTER_LEVEL,
                float moveSpeed = DEFAULT_CHARACTER_MOVESPEED);
        ~Monster();
};

#endif // MONSTER_H
