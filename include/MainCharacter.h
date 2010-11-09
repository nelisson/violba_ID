#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "Inventory.h"
#include <irrlicht/irrlicht.h>
#include "character.h"

#define WALK 0,14
#define IDLE 205,250
#define SPIN 45,59
#define PUNCH 31,45
#define KICK 73,83
#define SLASH 58,68
#define JUMP 94,102

#define STARTING_HP       1
#define STARTING_STRENGTH 1
#define STARTING_VITALITY 1
#define STARTING_AGILITY  1
#define STARTING_LEVEL    1

enum State{
    moving, idle2, attacking, jumping};

enum AttackType{
    spin, punch, kick, slash};


class MainCharacter : public Character {
    private:
        Inventory inventory_;

        int vitality_, strength_, agility_;
        long experience_;
        State state_;

    protected:

    public:
        void walk();
        void attack(AttackType type);
        void stop();
        void jump();

        virtual void levelUp();

        MainCharacter(string name,
                      char * modelPath,
                      int level = STARTING_LEVEL,
                      int maxHP = STARTING_HP,
                      int vitality = STARTING_VITALITY,
                      int strength = STARTING_STRENGTH,
                      int agility = STARTING_AGILITY);

        ~MainCharacter();
};

#endif // MAINCHARACTER_H
