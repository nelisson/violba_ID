#include "Character.h"

char * Character::getModelPath() {
    return modelPath_;
}

float Character::heal(float value) {
    currentHP_ += value;

    if (currentHP_ > maxHP_)
        currentHP_ = maxHP_;

    return value;
}

void Character::fillHP() {
    currentHP_ = maxHP_;
}

float Character::hurt(float value) {

    currentHP_ -= value;
    if (currentHP_ < 0)
        currentHP_ = 0;

    return value;
}

bool Character::isAlive() {
    return currentHP_ > 0;
}

Character::Character(string name,
                     char * modelPath,
                     int maxHP,
                     int level,
                     float moveSpeed)
    : Node() {
    name_  = name;
    modelPath_ = modelPath;
    level_ = level;
    maxHP_ = maxHP;
    moveSpeed_ = moveSpeed;

    fillHP();
}

Character::~Character() {}
