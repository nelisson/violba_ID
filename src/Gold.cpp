#include "Gold.h"

Gold::Gold(ISceneNode * parent,
               ISceneManager * manager,
               const std::string name,
               const char * modelPath,
               const char * imagePath,
               int amount)
    : Item(parent, manager,
           name, modelPath,
           imagePath),
      amount_(amount) {
}

Gold::~Gold() {}

Item* Gold::copy(ISceneNode * parent, ISceneManager * manager) {
    Item* item = new Gold(parent, manager, getNamex(), getModelPath().data(), getImagePath().data(), randomBetween(1, DEFAULT_MAX_GOLD));

    cout<<"vo loadamesh"<<endl;
    IAnimatedMesh * mesh = item->getSceneManager()->getMesh(item->getModelPath().data());
    cout<<"vo loada texture "<< getImagePath() <<endl;
    image_ = item->getSceneManager()->getVideoDriver()->getTexture(getImagePath().data());
    item->setImage(image_);
    printf("%Ponteiro da textura: %p\n",image_);
    cout<<"vo loada node"<<endl;
    item->setNode(item->getSceneManager()->addAnimatedMeshSceneNode(mesh, item, ISceneNode::getID()));
    cout<<"vo geta node"<<endl;
    item->getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);

    item->setScale(core::vector3df(0.02,0.02,0.02));
    item->setRotation(core::vector3df(90, 0, 0));
    item->setPosition(item->getPosition() + core::vector3df(50,50,50));
    return item;
}