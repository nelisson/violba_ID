#include "Monster.h"

int Monster::getExperienceGiven() {
    return experienceGiven_;
}

void Monster::levelUp() {
}

int Monster::getRange() {
    return 10;
}

Monster::Monster(string name, char * modelPath, int experienceGiven, int maxHP, int level)
    : Character(name, modelPath, maxHP, level) {
    experienceGiven_ = experienceGiven;
}

Monster::~Monster() {}
