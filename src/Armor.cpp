#include "Armor.h"

Armor::Armor(ISceneNode* parent,
             ISceneManager* manager,
             const std::string name,
             const char* modelPath)
    : Item(parent,
           manager,
           name,
           modelPath) {
}

Armor::~Armor() {}
