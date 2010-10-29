#ifndef MONSTER_H
#define MONSTER_H

#include "character.h"

class Monster : public Character {
    private:
        int experienceGiven_;

    protected:

    public:
        int getExperienceGiven();
        virtual void levelUp();

        Monster(string name, char * modelPath, int experienceGiven, int maxHP, int level);
        virtual ~Monster();
};

#endif // MONSTER_H
