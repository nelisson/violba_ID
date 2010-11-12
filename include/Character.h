#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <irrlicht/irrlicht.h>
#include "node.h"

#define DEFAULT_CHARACTER_LEVEL 1
#define DEFAULT_CHARACTER_MOVESPEED 100

using namespace std;
using namespace irr::scene;

class Character : public Node {
    private:
        string name_;
        char * modelPath_;
        int level_;
        int maxHP_;
        float currentHP_;
        float moveSpeed_;

    protected:

    public:
        char * getModelPath();
        void fillHP();
        float heal(float value);
        float hurt(float value);
        bool isAlive();
        float getMoveSpeed() { return moveSpeed_; }

        float getHPPercentual() { return 100 * currentHP_/(float)maxHP_; }
        virtual float getDamage() = 0;

        virtual void levelUp() = 0;

        Character(string name,
                  char * modelPath,
                  int maxHP,
                  int level = DEFAULT_CHARACTER_LEVEL,
                  float moveSpeed = DEFAULT_CHARACTER_MOVESPEED);
        virtual ~Character();
};

#endif // CHARACTER_H
