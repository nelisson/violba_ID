#include "MainCharacter.h"

void MainCharacter::walk() {}

void MainCharacter::slash(void * userData) {

    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() == STOPPING) {
        thisptr->getNode()->setFrameLoop(SLASH);
        thisptr->setState(ATTACKING);
    }
}

void MainCharacter::stop() {}

void MainCharacter::jump(void * userData) {

    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() == STOPPING) {
        thisptr->getNode()->setFrameLoop(JUMP);
        thisptr->setState(JUMPING);
    }
}

void MainCharacter::levelUp() {}

void MainCharacter::setState(State state) {
    if(state_ == STOPPING)
        state_ = state;
}

MainCharacter::MainCharacter(string name,
                             char * modelPath,
                             int level,
                             int maxHP,
                             int vitality,
                             int strength,
                             int agility)
    : Character(name, modelPath, level, maxHP) {

    vitality_ = vitality;
    strength_ = strength;
    agility_  = agility;
}

MainCharacter::~MainCharacter()
{
    //dtor
}
