#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <irrlicht/irrlicht.h>
#include <iostream>
#include "node.h"
#include "bar.h"

#define DEFAULT_CHARACTER_LEVEL 1
#define DEFAULT_CHARACTER_MOVESPEED 60

using namespace std;
using namespace irr::scene;

class Character : public Node, public ISceneNode {
    private:
        std::string name_;
        int level_;
        int maxHP_;
        float currentHP_;
        float moveSpeed_;
        Bar * healthBar_;

    protected:

    public:

        void fillHP();
        float heal(float value);
        float hurt(float value);
        float getHPPercentual() { return 100 * currentHP_/(float)maxHP_; }
        bool isAlive();

        float getMoveSpeed() { return moveSpeed_; }
        void setMoveSpeed(float moveSpeed) { moveSpeed_ = moveSpeed; }
        void increaseMoveSpeed(float moveSpeed) { moveSpeed_ += moveSpeed; }

        float getMaxHP() { return maxHP_; }
        void setMaxHP(int maxHP) { maxHP_ = maxHP; }
        void increaseMaxHP(int maxHPIncrease) { maxHP_ += maxHPIncrease; }

        int getLevel() { return level_; }
        void setLevel(int level) { level_ = level; }
        void addLevels(int levels) { level_+= levels; }

        void moveDelta(core::vector3df delta) { setPosition(getPosition() + delta); }
        void moveTo(core::vector3df position) { setPosition(position); }

        virtual float getDamage() = 0;
        virtual void levelUp() = 0;
        virtual bool walk(vector3df delta) = 0;

        virtual void render();
        const core::aabbox3d<f32>& getBoundingBox() const;

        Character(ISceneNode * parent,
                  ISceneManager * manager,
                  std::string name,
                  char * modelPath,
                  int maxHP,
                  int level = DEFAULT_CHARACTER_LEVEL,
                  float moveSpeed = DEFAULT_CHARACTER_MOVESPEED);

        virtual ~Character();
};

#endif // CHARACTER_H
