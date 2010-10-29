#include "MainCharacter.h"

void MainCharacter::walk() {}

void MainCharacter::attack(AttackType type) {}

void MainCharacter::stop() {}

void MainCharacter::jump() {}

void MainCharacter::levelUp() {}

MainCharacter::MainCharacter(string name,
                             int level,
                             int maxHP,
                             int vitality,
                             int strength,
                             int agility)
    : Character(name, level, maxHP) {
    vitality_ = vitality;
    strength_ = strength;
    agility_  = agility;
}

MainCharacter::~MainCharacter()
{
    //dtor
}
