#include "Monster.h"

int Monster::getExperienceGiven() {
    return experienceGiven_;
}

void Monster::levelUp() {
}

Monster::Monster(string name, char * modelPath, int experienceGiven, int maxHP, int level)
    : Character(name, modelPath, maxHP, level) {
    experienceGiven_ = experienceGiven;
}

Monster::~Monster() {
    //dtor
}
