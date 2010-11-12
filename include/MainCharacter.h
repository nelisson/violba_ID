#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "Inventory.h"
#include <irrlicht/irrlicht.h>
#include "character.h"
#include <iostream>
#include "Utils.h"
#include "weapon.h"

#define WALK 0,14
#define IDLE 205,250
#define SPIN 45,59
#define PUNCH 31,45
#define KICK 73,83
#define SLASH 58,68
#define JUMP 94,102

#define DEFAULT_CHARACTER_NAME "Violba"
#define DEFAULT_CHARACTER_MESH "./models/ninja.b3d"

#define STARTING_HP       1
#define STARTING_STRENGTH 1
#define STARTING_VITALITY 1
#define STARTING_AGILITY  1
#define STARTING_LEVEL    1

using namespace irr;

enum State{
    MOVING, STOPPING, ATTACKING, JUMPING};


class MainCharacter : public Character, public IAnimationEndCallBack {
    private:
        Inventory inventory_;
        Weapon * equippedWeapon_;

        int vitality_, strength_, agility_;
        long experience_;
        State state_;
        f32 speed_;

    protected:

    public:
        bool walk(core::vector3df desl);
        static void slash(void *userData);
        static void spin(void *userData);
        static void kick(void *userData);
        static void stop(void *userData, core::vector2df direction);
        static void jump(void *userData);

        virtual void levelUp();

        MainCharacter(char * name = DEFAULT_CHARACTER_NAME,
                      char * meshPath = DEFAULT_CHARACTER_MESH,
                      int level = STARTING_LEVEL,
                      int maxHP = STARTING_HP,
                      int vitality = STARTING_VITALITY,
                      int strength = STARTING_STRENGTH,
                      int agility = STARTING_AGILITY,
                      float moveSpeed = DEFAULT_CHARACTER_MOVESPEED);

        ~MainCharacter();

        State getState() { return state_; }
        void setState(State state);

        Weapon * getEquippedWeapon() { return equippedWeapon_; }
        virtual float getDamage();

        void OnAnimationEnd(IAnimatedMeshSceneNode *node) {
            node->setFrameLoop(IDLE);
            state_ = STOPPING;
        }
};

#endif // MAINCHARACTER_H
