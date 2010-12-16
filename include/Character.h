#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <irrlicht/irrlicht.h>
#include <iostream>
#include "AnimatedNode.h"
#include "Bar.h"
#include "SoundEmmitter.h"

#define DEFAULT_CHARACTER_LEVEL 1
#define DEFAULT_CHARACTER_MOVESPEED 60

using namespace std;
using namespace irr::scene;
using namespace irrklang;

namespace State {
    enum State{
        MOVING,
        RUNNING,
        STOPPING,
        ATTACK_STARTING,
        ATTACK_ENDING,
        JUMPING,
        DOUBLE_JUMPING,
        BLOCKING,
        CROUCHING,
        DYING,
        DEAD,
    };
}

namespace Sounds {
    enum Sounds {
        POTION,
        HURT,
        LEVEL_UP,

        SWING1,
        DEAD,

        SWING2,
        TIME_TO_DIE,
        WALK1,
        KICK,
        SPIN,
        JUMP,
        BLOCK,
        ITEM_DROP = 0,
        SELECTION = 1,
        GOLD_DROP = 2,
    };
}

class Character : public AnimatedNode,
                  public ISceneNode,
                  public SoundEmmitter,
                  public IAnimationEndCallBack {

    private:
        State::State state_;
        std::string name_;
        dimension2df size_;
        int level_;
        int maxHP_;
        float currentHP_;
        float moveSpeed_;
        Bar * healthBar_;

    protected:

    public:
        State::State getState() const     { return state_; }
        void setState(State::State state) { state_ = state; }

        position2di getGridPosition() const;

        void setSize(dimension2df size) { size_ = size; }
        dimension2df getSize() const    { return size_; }

        float getCurrentHP() {return currentHP_; };
        
        void fillHP();
        float heal(float value, bool playSound = true);
        float hurt(float value);
        float getHPPercentual() const { return 100 * currentHP_/(float)maxHP_; }
        bool isAlive() const          { return currentHP_ > 0; }

        float getMoveSpeed() const              { return moveSpeed_; }
        void setMoveSpeed(float moveSpeed)      { moveSpeed_ = moveSpeed; }
        void increaseMoveSpeed(float moveSpeed) { moveSpeed_ += moveSpeed; }

        float getMaxHP() const                { return maxHP_; }
        void setMaxHP(int maxHP)              { maxHP_ = maxHP; }
        void increaseMaxHP(int maxHPIncrease) { maxHP_ += maxHPIncrease; }

        int getLevel() const       { return level_; }
        void setLevel(int level)   { level_ = level; }
        void addLevels(int levels) { level_+= levels; }

        void moveDelta(core::vector3df delta) { setPosition(getPosition() + delta); }
        void moveTo(core::vector3df position) { setPosition(position); }

        virtual float getDamage() const = 0;
        virtual void levelUp() = 0;
        virtual bool walk(vector3df delta) = 0;
        virtual void die() = 0;
        virtual void refresh() = 0;
        virtual void OnAnimationEnd(IAnimatedMeshSceneNode *node) = 0;

        virtual void render();
        virtual const core::aabbox3d<f32>& getBoundingBox() const { }

        Character(ISceneNode * parent,
                  ISceneManager * manager,
                  ISoundEngine * soundEngine,
                  vector3df offset,
                  const std::string name,
                  const char * modelPath,
                  int level = DEFAULT_CHARACTER_LEVEL);

        virtual ~Character();
};

#endif // CHARACTER_H
