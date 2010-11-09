#include "Character.h"

char * Character::getModelPath() {
    return modelPath_;
}

void Character::heal(int value) {
    currentHP_ += value;

    if (currentHP_ > maxHP_)
        currentHP_ = maxHP_;
}

void Character::fillHP() {
    currentHP_ = maxHP_;
}

void Character::hurt(int value) {
    currentHP_ -= value;

    if (currentHP_ < 0)
        currentHP_ = 0;
}

bool Character::isAlive() {
    return currentHP_ > 0;
}

Character::Character(string name, char * modelPath, int maxHP, int level)
    : Node() {
    name_  = name;
    modelPath_ = modelPath;
    level_ = level;
    maxHP_ = maxHP;

    fillHP();
}

Character::~Character() {
    //dtor
}
