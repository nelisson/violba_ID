#include "Item.h"

int Item::id_ = 0;

Item::Item(const Item & oldItem,
           ISceneNode * parent,
           ISceneManager * manager)
    : level_(oldItem.level_),
      ID_(oldItem.id_),
      name_(oldItem.name_),
      AnimatedNode(), ISceneNode(parent, manager) {}

Item::Item(ISceneNode * parent,
           ISceneManager * manager,
           const string name,
           const char * modelPath,
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
