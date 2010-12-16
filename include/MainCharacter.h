#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "Inventory.h"
#include <irrlicht/irrlicht.h>
#include "Character.h"
#include <iostream>
#include <cstdlib>
#include "Utils.h"
#include <irrklang/irrKlang.h>
#include "Weapon.h"

#define ANIM_WALK  0,13
#define ANIM_SLOW_WALK  14,29
#define ANIM_PUNCH 31,43
#define ANIM_SPIN  44,58
#define ANIM_SLASH 59,67
#define ANIM_BLOCK 68,71
#define ANIM_UNBLOCK 71,68
#define ANIM_KICK  72,82
#define ANIM_BLOCKFREEZE 71,71
#define ANIM_PICK_ITEM 83,92
#define ANIM_CROUCH 83,86
#define ANIM_GET_UP 86,92
#define ANIM_CROUCHFREEZE 86,86
//#define JUMP 94,102 // with height
#define ANIM_JUMP_START 102,106  // without height
#define ANIM_JUMP_END 106,110  // without height
#define ANIM_BACKFLIP 145,157
#define ANIM_FRONTFLIP 157,145
#define ANIM_DEATH_BACKWARDS 165,172
#define ANIM_DEATH_FORWARDS  173,181
#define ANIM_IDLE  205,249

#define DEFAULT_CHARACTER_NAME "Violba"
//#define DEFAULT_CHARACTER_MESH "./models/ninja.b3d"
#define DEFAULT_CHARACTER_MESH "./models/ninja.b3d"

#define DEFAULT_CHARACTER_MAX_LEVEL 40
#define DEFAULT_POTION_HEAL 100

#define STARTING_EXPERIENCE  0
#define STARTING_HP          100
#define STARTING_STRENGTH    1
#define STARTING_VITALITY    1
#define STARTING_AGILITY     1
#define STARTING_LEVEL       1
#define STARTING_JUMP_HEIGHT 30
#define JUMP_ACCELERATION 30

using namespace std;
using namespace irr;
using namespace irr::scene;
using namespace irrklang;

class MainCharacter : public Character {
    
    private:
        Inventory* inventory_;
        Weapon* equippedWeapon_;

        int vitality_, strength_, agility_;
        long currentExperience_;
        long experienceToLevelUp_;
        f32 speed_;
        float jumpHeight_;

        long experienceCurve(int level) const { return (level == 0)? 0 : 980 + 200 * level*level; };

    protected:

    public:

        static void slash(void *userData);
        static void spin(void *userData);
        static void kick(void *userData);
        static void stop(void *userData, vector2df direction);
        static void jump(void *userData);
        static void doubleJump(void *userData);
        static void crouch(void *userData);
        static void getUp(void *userData);
        static void block(void *userData);
        static void unblock(void *userData);
        static void drinkPotion(void *userData);

        virtual void levelUp();


        int getVitality() {return vitality_;}
        int getStrength() {return strength_;}
        int getAgility() {return agility_;}

        Inventory* getInventory() const { return inventory_; }


        float getJumpHeight() const { return jumpHeight_; }
        void earnExperience(int experience);

        void updateAttributes();
        bool tryHitCheck();

        Weapon * getEquippedWeapon() const { return equippedWeapon_; }
        virtual float getDamage() const;
        virtual bool walk(vector3df desl);
        virtual void die();
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node);
        virtual void refresh(f32 elapsedTime);

        MainCharacter(ISceneNode* parent,
                      ISceneManager* manager,
                      ISoundEngine * soundEngine,
                      vector3df offset = vector3df(),
                      const char * name = DEFAULT_CHARACTER_NAME,
                      const char * meshPath = DEFAULT_CHARACTER_MESH,
                      int level = STARTING_LEVEL);

        ~MainCharacter();
};

#endif // MAINCHARACTER_H
