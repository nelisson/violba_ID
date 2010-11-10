#include "MainCharacter.h"

bool MainCharacter::walk(core::vector3df desl) {

    if(getState() == STOPPING || getState() == MOVING || getState() == JUMPING) {
        float moveHorizontal = desl.X;
        float moveVertical = desl.Z;
        double ang = sinal(moveHorizontal) * (180.0/3.1415) * acos(core::vector3df(0,0,1).dotProduct(core::vector3df(moveHorizontal,0,moveVertical).normalize()));
        if (ang >= 180)
            ang = 179;
        getNode()->setRotation(core::vector3df(0,ang,0));
        core::vector3df nodePosition = getNode()->getPosition();

        getNode()->setPosition(nodePosition + desl);
        if (getState() == STOPPING) {
            getNode()->setFrameLoop(WALK);
            setState(MOVING);
        }

        return true;
    }
    return false;
}

void MainCharacter::slash(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACKING) {
        thisptr->getNode()->setFrameLoop(SLASH);
        thisptr->setState(ATTACKING);
    }
}

void MainCharacter::kick(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACKING) {
        thisptr->getNode()->setFrameLoop(KICK);
        thisptr->setState(ATTACKING);
    }
}

void MainCharacter::spin(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACKING) {
        thisptr->getNode()->setFrameLoop(SPIN);
        thisptr->setState(ATTACKING);
    }
}

void MainCharacter::stop(void *userData, core::vector2df direction) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    thisptr->getNode()->setFrameLoop(IDLE);
    thisptr->setState(STOPPING);
}

void MainCharacter::jump(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACKING) {
        thisptr->getNode()->setFrameLoop(JUMP);
        thisptr->setState(JUMPING);
    }
}

void MainCharacter::levelUp() {}

void MainCharacter::setState(State state) {
    state_ = state;
}

float MainCharacter::getDamage() {
    return getEquippedWeapon()->getDamage() * strength_;
}

MainCharacter::MainCharacter(char * name,
                             char * modelPath,
                             int level,
                             int maxHP,
                             int vitality,
                             int strength,
                             int agility,
                             float moveSpeed)
    : Character(name, modelPath, level, maxHP, moveSpeed) {

    vitality_ = vitality;
    strength_ = strength;
    agility_  = agility;

    equippedWeapon_ = new Weapon("Espada");

    state_ = STOPPING;
}

MainCharacter::~MainCharacter() {
    delete equippedWeapon_;
}
