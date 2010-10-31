#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <irrlicht.h>

#define DEFAULT_CHARACTER_LEVEL 1

using namespace std;
using namespace irr::scene;

class Character { // : public IAnimatedMeshSceneNode {
    private:
        string name_;
        char * modelPath_;
        IAnimatedMeshSceneNode* model_;
        int level_;
        int maxHP_;
        int currentHP_;

    protected:

    public:
        IAnimatedMeshSceneNode* getModel();
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
