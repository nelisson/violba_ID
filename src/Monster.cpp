#include "Monster.h"

void Monster::levelUp() {}

bool Monster::walk(vector3df delta) {
    moveDelta(delta * getMoveSpeed());

    float angle = 180/PI * acos(delta.normalize().dotProduct(vector3df(1.0, 0.0, 0.0)));
    setRotation( vector3df(0.0, -angle, 0.0));
}

bool Monster::canAttack() {

    time_t currentTime;
    time(&currentTime);

    //cout << "Difftime: " << difftime(currentTime, attackStart_) << endl;

    return (difftime(currentTime, attackStart_) >= 2);
}

void Monster::attack() {

    time(&attackStart_);
}

Monster::Monster(ISceneNode * parent,
                 ISceneManager * manager,
                 std::string name,
                 char * modelPath,
                 int experienceGiven,
                 int maxHP,
                 int range,
                 int level,
                 float moveSpeed,
                 float attackSpeed,
                 float minDamage,
                 float maxDamage)
    : Character(parent, manager, name, modelPath, maxHP, level, moveSpeed),
      experienceGiven_(experienceGiven),
      range_(range) {

    time(&attackStart_);
    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    attackSpeed_ = attackSpeed;
    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setAnimationSpeed(30);
    setLoopMode(false);
    setScale(core::vector3df(0.1, 0.1, 0.1));
    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
}

Monster::~Monster() {}
