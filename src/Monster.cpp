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
        setLoopMode(false);
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
                 const char * modelPath,
                 const std::string name,
                 int experienceGiven,
                 int maxHP,
                 int range,
                 int level,
                 float moveSpeed,
                 float attackSpeed,
                 float minDamage,
                 float maxDamage)
    : Character(parent, manager, NodeIDFlags::ENEMY,
                soundEngine, vector3df(0,0,0), name,
                modelPath, level),
      experienceGiven_(experienceGiven),
      range_(range), corpseDelay_(DEFAULT_MONSTER_CORPSE_DELAY) {

    time(&attackStart_);
    minDamage_ = minDamage;
    maxDamage_ = maxDamage;
    attackSpeed_ = attackSpeed;

    setMaxHP(DEFAULT_MONSTER_HP);
    setMoveSpeed(DEFAULT_CHARACTER_MOVESPEED);

    addSoundEffect("./sounds/potion.wav");
    addSoundEffect("./sounds/monsterHit.wav");
    addSoundEffect("./sounds/levelUp.wav");
    addSoundEffect("./sounds/monsterSwing.wav");
    addSoundEffect("./sounds/monsterDead.wav");

    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setAnimationSpeed(20);
    setLoopMode(false);
    setAnimationEndCallback(this);
    setFrameLoop(MONSTER_IDLE);
    setState(STOPPING);
    setScale(core::vector3df(0.1, 0.1, 0.1));
}

Monster::~Monster() {}
