#include "Monster.h"

using namespace State;

void Monster::levelUp() {}

bool Monster::walk(vector3df delta) {
    float moveHorizontal = delta.X;
    float moveVertical = delta.Z;
    double ang = sinal(moveHorizontal) * (180.0 / PI) * acos(vector3df(0, 0, 1).dotProduct(vector3df(moveHorizontal, 0, moveVertical).normalize()));
    if (ang >= 180)
        ang = 179;

    setRotation(vector3df(0, ang, 0));

    moveDelta(delta * getMoveSpeed());
}

void Monster::die() {
    if (getState() != DYING) {
        setFrameLoop(MONSTER_DIE);
        setState(DYING);
        playSoundEffect(Sounds::DEAD);
    }
}

void Monster::OnAnimationEnd(IAnimatedMeshSceneNode *node) {
    switch (getState()) {
        case DYING :
        case DEAD :
            setState(DEAD);
            break;

        default :
            setFrameLoop(MONSTER_IDLE);
    }
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
                soundEngine, vector3df(0,0,0), name,
                modelPath, maxHP,
                level, moveSpeed),
      experienceGiven_(experienceGiven),
      range_(range) {

    time(&attackStart_);
    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    attackSpeed_ = attackSpeed;

    addSoundEffect("./sounds/potion.wav");
    addSoundEffect("./sounds/monsterHit.wav");
    addSoundEffect("./sounds/levelUp.wav");
    addSoundEffect("./sounds/monsterSwing.wav");
    addSoundEffect("./sounds/monsterDead.wav");

    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setAnimationSpeed(30);
    setLoopMode(false);
    setAnimationEndCallback(this);
    setState(STOPPING);
    setScale(core::vector3df(0.1, 0.1, 0.1));
}

Monster::~Monster() {}
