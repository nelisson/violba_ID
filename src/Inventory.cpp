#include "Inventory.h"

void Inventory::setMaxItems(int maxItems) {
    maxItems_ = maxItems;
}

void Inventory::increaseSize(int size) {
    maxItems_ += size;
}

Item * Inventory::removeItem(int) {
    //slots_
}

void Inventory::putItem(Item * item) {
    try {
        findEmptySlot()->putItem(item);
        cout << "putei item fora" << endl;
    }
    catch (int i) {
        throw;
    }
}

Slot* Inventory::findEmptySlot() const {
    vector<Slot*>::const_iterator i;

    for (i = slots_.begin(); i < slots_.end(); i++) {
        cout << "for slots" << endl;
        printf("%p\n", *i);
        cout << endl;
        if ((*i)->isEmpty()) {
            cout << "is empty" << endl;
            return *i;
        }
        else {
            cout << "not empty" << endl;
        }
    }

    throw 2;
}

Item * Inventory::putItem(Item *, int) {

}

Inventory::Inventory(ISceneManager* sceneManager, int maxItems)
    : sceneManager_(sceneManager), maxItems_(maxItems) {

    for (int i = 0; i < maxItems_; i++) {
        slots_.push_back(new Slot());
        printf("%p\n", slots_[i]);
    }

}

Inventory::~Inventory() {
    //dtor
}

void Inventory::drawInventory(){
    gui::IGUIEnvironment* env = sceneManager_->getGUIEnvironment();
    sceneManager_->getVideoDriver()->draw2DRectangle(SColor(255, 50, 50, 50), rect<s32 > (500, 150, 1000, 650));

    vector<Slot*>::const_iterator i;
    int deslocX = 0, deslocY = 0;
    for(i = slots_.begin(); i< slots_.end();i++){

        if(!(*i)->isEmpty()){

            ITexture* teste = (*i)->getItem()->getImage();
            

            if(teste){                
                env->addImage(teste, position2di(501+(deslocX*100),151+(deslocY*100)));
            }

        }
        deslocX++;
        if(deslocX%5==0){
            deslocY++;
            deslocX=0;
        }
    }

    for(int i=1;i<5;i++){
        sceneManager_->getVideoDriver()->draw2DLine(position2di(500+(100*i),150),position2di(500+(100*i),650));
        sceneManager_->getVideoDriver()->draw2DLine(position2di(500,150+(100*i)),position2di(1000,150+(100*i)));
    }
}
