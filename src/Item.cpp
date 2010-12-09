#include "Item.h"

int Item::id_ = 0;

int Item::generateID() {
    return id_++;
}

int Item::getLevel() {
    return level_;
}

int Item::getID() {
    return ID_;
}

string Item::getName() {
    return name_;
}

Item::Item(const Item & oldItem,
           ISceneNode * parent,
           ISceneManager * manager)
    : level_(oldItem.level_),
      ID_(oldItem.id_),
      name_(oldItem.name_),
      AnimatedNode(), ISceneNode(parent, manager) {}

Item::Item(ISceneNode * parent,
           ISceneManager * manager,
           string name,
           char * modelPath,
           int level)
    : AnimatedNode(), ISceneNode(parent, manager) {
    
    if(manager) {
        IAnimatedMesh * mesh = getSceneManager()->getMesh(modelPath);
        setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh, this));
    }

    level_ = level;
    ID_    = generateID();
    name_  = name;
}

Item::~Item() {
    //dtor
}
