#include "Armor.h"

Armor::Armor(ISceneNode* parent,
             ISceneManager* manager,
             const std::string name,
             const char* modelPath)
    : Item(parent,
           manager,
           name,
           modelPath) {

    setScale(core::vector3df(50,50,50));
}

Armor::~Armor() {}
