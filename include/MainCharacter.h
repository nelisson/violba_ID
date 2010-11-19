#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "Inventory.h"
#include <irrlicht/irrlicht.h>
#include "Character.h"
#include <iostream>
#include "Utils.h"
#include "Weapon.h"

#define WALK 0,14
#define IDLE 205,250
#define SPIN 45,59
#define PUNCH 31,45
#define KICK 73,83
#define SLASH 58,68
//#define JUMP 94,102 // with height
#define JUMP 102,111  // without height

#define DEFAULT_CHARACTER_NAME "Violba"
#define DEFAULT_CHARACTER_MESH "./models/ninja.b3d"

#define DEFAULT_CHARACTER_MAX_LEVEL 40

#define STARTING_EXPERIENCE  0
#define STARTING_HP          1
#define STARTING_STRENGTH    1
#define STARTING_VITALITY    1
#define STARTING_AGILITY     1
#define STARTING_LEVEL       1
#define STARTING_JUMP_HEIGHT 30

using namespace std;
using namespace irr;
using namespace irr::scene;

enum State{
    MOVING, STOPPING, ATTACK_STARTING, ATTACK_ENDING, JUMPING,
};


class MainCharacter : public Character, public IAnimationEndCallBack {
    private:
        Inventory inventory_;
        Weapon * equippedWeapon_;

        int vitality_, strength_, agility_;
        long currentExperience_;
        long experienceToLevelUp_;
        State state_;
        f32 speed_;
        float jumpHeight_;

        long experienceCurve(int level);

    protected:

    public:
        static void slash(void *userData);
        static void spin(void *userData);
        static void kick(void *userData);
        static void stop(void *userData, vector2df direction);
        static void jump(void *userData);

        virtual void levelUp();

        State getState() { return state_; }
        void setState(State state);

        float getJumpHeight() {return jumpHeight_; }
        void earnExperience(int experience);

        void updateAttributes();
        bool tryHitCheck();

        Weapon * getEquippedWeapon() { return equippedWeapon_; }
        virtual float getDamage();
        virtual bool walk(vector3df desl);
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node);
        virtual void refresh();

        MainCharacter(ISceneNode* parent,
                      ISceneManager* manager,
                      char * name = DEFAULT_CHARACTER_NAME,
                      char * meshPath = DEFAULT_CHARACTER_MESH,
                      int level = STARTING_LEVEL,
                      int currentExperience = STARTING_EXPERIENCE,
                      int maxHP = STARTING_HP,
                      int vitality = STARTING_VITALITY,
                      int strength = STARTING_STRENGTH,
                      int agility = STARTING_AGILITY,
                      float moveSpeed = DEFAULT_CHARACTER_MOVESPEED,
                      float jumpHeight = STARTING_JUMP_HEIGHT);

        ~MainCharacter();
};

#endif // MAINCHARACTER_H
