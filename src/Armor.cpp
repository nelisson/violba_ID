#include "Armor.h"

Armor::Armor(ISceneNode* parent,
             ISceneManager* manager,
             const std::string name,
             const char* modelPath,
             const char* imagePath)
    : Item(parent,
           manager,
           name,
           modelPath,
           imagePath) {    
}

Armor::~Armor() {}
