#include "MainCharacter.h"

void MainCharacter::walk(void* userData, core::vector2df desl) {

    MainCharacter * thisptr = (MainCharacter*) userData;
    core::vector3df nodePosition = thisptr->getNode()->getPosition();
    desl.normalize();
    float moveHorizontal = desl.X;
    float moveVertical =desl.Y;
    double ang = sinal(moveHorizontal) *(180.0/3.1415)*acos(core::vector3df(0,0,1).dotProduct(core::vector3df(moveHorizontal,0,moveVertical).normalize()));


    thisptr->getNode()->setRotation(core::vector3df(0,ang,0));
    nodePosition.X += thisptr->getSpeed() *0.0007*  moveHorizontal;
    nodePosition.Z += thisptr->getSpeed() *0.0007*  moveVertical;

    thisptr->getNode()->setPosition(nodePosition);
}

void MainCharacter::slash(void * userData) {

    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() == STOPPING) {
        thisptr->getNode()->setFrameLoop(SLASH);
        thisptr->setState(ATTACKING);
    }
}
void MainCharacter::spin(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() == STOPPING) {
        thisptr->getNode()->setFrameLoop(SPIN);
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
    speed_ = 10.f;
}

MainCharacter::~MainCharacter()
{
    //dtor
}
