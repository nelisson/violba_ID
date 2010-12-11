#include "Monster.h"

using namespace State;

void Monster::levelUp() {}

bool Monster::walk(vector3df delta) {
    moveDelta(delta * getMoveSpeed());

    float angle = 180/PI * acos(delta.normalize().dotProduct(vector3df(1.0, 0.0, 0.0)));
    setRotation( vector3df(0.0, -angle, 0.0));
}

void Monster::die() {
    if (getState() != DYING) {
        setFrameLoop(MONSTER_DIE);
        setState(DYING);
        playSoundEffect(MonsterSound::DEAD);
    }
}

void Monster::OnAnimationEnd(IAnimatedMeshSceneNode *node) {
    if (getState() == DYING) {
        setState(DEAD);
    }
    else
        setFrameLoop(MONSTER_IDLE);
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
                 ISoundEngine * soundEngine,
                 const std::string name,
                 const char * modelPath,
                 int experienceGiven,
                 int maxHP,
                 int range,
                 int level,
                 float moveSpeed,
                 float attackSpeed,
                 float minDamage,
                 float maxDamage)
    : Character(parent, manager,
                soundEngine, name,
                modelPath, maxHP,
                level, moveSpeed),
      experienceGiven_(experienceGiven),
      range_(range) {

    time(&attackStart_);
    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    attackSpeed_ = attackSpeed;

    vector<std::string> sounds;

    sounds.push_back("./sounds/swing2.wav");
    sounds.push_back("./sounds/falld1.wav");

    addSoundEffects(sounds);

    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setAnimationSpeed(300);
    setLoopMode(false);
    setAnimationEndCallback(this);
    setState(STOPPING);
    setScale(core::vector3df(0.1, 0.1, 0.1));
}

Monster::~Monster() {}
