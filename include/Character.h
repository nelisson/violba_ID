#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <irrlicht/irrlicht.h>
#include "node.h"

#define DEFAULT_CHARACTER_LEVEL 1

using namespace std;
using namespace irr::scene;

class Character : public Node {
    private:
        string name_;
        char * modelPath_;
        int level_;
        int maxHP_;
        int currentHP_;

    protected:

    public:
        char * getModelPath();
        void fillHP();
        void heal(int value);
        void hurt(int value);
        bool isAlive();

        virtual void levelUp() = 0;

        Character(string name, char * modelPath, int maxHP, int level = DEFAULT_CHARACTER_LEVEL);
        virtual ~Character();
};

#endif // CHARACTER_H
