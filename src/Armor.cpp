#include "Armor.h"

Armor::Armor(ISceneNode * parent,
             ISceneManager * manager,
             std::string name,
             char * modelPath)
    : Item(parent,
           manager,
           name,
           modelPath) {
}

Armor::~Armor() {}
