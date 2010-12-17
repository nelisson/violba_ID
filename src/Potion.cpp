#include "Weapon.h"
#include "Potion.h"

Potion::Potion(ISceneNode * parent,
               ISceneManager * manager,
               std::string name,
               const char * modelPath,
               const char * imagePath,
               int healAmount)
    : Item(parent,
           manager,
           name,
           modelPath,
           imagePath) {

    healAmount_ = healAmount;

    
}

Potion::~Potion() {}

Item* Potion::copy(ISceneNode * parent, ISceneManager * manager) {
    Item* item = new Potion(parent, manager, getNamex(), getModelPath().data(), getImagePath().data());

    cout<<"vo loadamesh"<<endl;
    IAnimatedMesh * mesh = item->getSceneManager()->getMesh(item->getModelPath().data());
    cout<<"vo loada texture "<< getImagePath() <<endl;
    image_ = item->getSceneManager()->getVideoDriver()->getTexture(getImagePath().data());
    item->setImage(image_);
    printf("%Ponteiro da textura: %p\n",image_);
    cout<<"vo loada node"<<endl;
    item->setNode(item->getSceneManager()->addAnimatedMeshSceneNode(mesh, item));
    cout<<"vo geta node"<<endl;
    item->getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);

    item->setScale(core::vector3df(5,5,5));
    item->setRotation(core::vector3df(0,45,0));
    return item;
}