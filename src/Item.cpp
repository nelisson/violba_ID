#include "Item.h"

int Item::id_ = 0;

Item::Item(ISceneNode * parent,
           ISceneManager * manager,
           const string name,
           const char * modelPath,
           const char* imagePath,
           int level
           )
    : AnimatedNode(), ISceneNode(parent, manager),
      modelPath_(modelPath), level_(level),
      name_(name), imagePath_(imagePath) {
    ID_ = generateID();
}

Item* Item::copy(ISceneNode * parent, ISceneManager * manager) {
    Item* item = new Item(parent, manager, getNamex(), modelPath_.data(), imagePath_.data());

    cout<<"vo loadamesh"<<endl;
    IAnimatedMesh * mesh = item->getSceneManager()->getMesh(item->modelPath_.data());
    cout<<"vo loada texture "<< imagePath_ <<endl;
    image_ = item->getSceneManager()->getVideoDriver()->getTexture(imagePath_.data());
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



Item::~Item() {
    //dtor
}
