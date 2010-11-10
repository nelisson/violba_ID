#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "Inventory.h"
#include <irrlicht/irrlicht.h>
#include "character.h"
#include <iostream>
#include "Utils.h"

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

        int vitality_, strength_, agility_;
        long experience_;
        State state_;
        f32 speed_;

    protected:

    public:
        static void walk(void *, irr::core::vector2df);
        static void slash(void *);
        static void spin(void *);
        void stop();
        static void jump(void *);

        virtual void levelUp();

        MainCharacter(string name = DEFAULT_CHARACTER_NAME,
                      char * meshPath = DEFAULT_CHARACTER_MESH,
                      int level = STARTING_LEVEL,
                      int maxHP = STARTING_HP,
                      int vitality = STARTING_VITALITY,
                      int strength = STARTING_STRENGTH,
                      int agility = STARTING_AGILITY);

        ~MainCharacter();

        State getState() { return state_; }
        void setState(State state);

        f32 getSpeed() { return speed_; }

        void OnAnimationEnd(IAnimatedMeshSceneNode *node) {
            node->setFrameLoop(IDLE);
            state_ = STOPPING;
        }
};

#endif // MAINCHARACTER_H
