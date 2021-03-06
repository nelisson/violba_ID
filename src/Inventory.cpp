#include "Inventory.h"
#include "Game.h"

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
        
    }
    catch (int i) {
        throw;
    }
}

Slot* Inventory::findEmptySlot() const {
    vector<Slot*>::const_iterator i;

    for (i = slots_.begin(); i < slots_.end(); i++) {
        
        if ((*i)->isEmpty()) {
        
            return *i;
        }
        
    }

    throw 2;
}

Item * Inventory::putItem(Item *, int) {

}

void Inventory::clear() {
    vector<Slot*>::const_iterator i;
    for (i = slots_.begin(); i < slots_.end(); i++)
        (*i)->clear();
}

Inventory::Inventory(ISceneManager* sceneManager, int maxItems)
    : sceneManager_(sceneManager), maxItems_(maxItems) {

    for (int i = 0; i < maxItems_; i++) {
        slots_.push_back(new Slot());
        
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
                if((*i)->getItem()->getNamex().compare("Potion")==0)
                    env->addImage(teste, position2di(501+(deslocX*100),151+(deslocY*100)),true,NULL,GUI_ID_POTION_IMAGE);
                else
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


Potion* Inventory::getPotion(){
    
    vector<Slot*>::const_iterator i;
    for(i = slots_.begin(); i < slots_.end(); i++){

        if(!(*i)->isEmpty()){
            if((*i)->getItem()->getNamex().compare("Potion")==0){

                Potion* pot = dynamic_cast<Potion*>( removeItem(i));
                
                printf("Potion pointer %p\n",pot);

                return pot;
            }
        }
    }
    
    return NULL;
}

Item * Inventory::removeItem(vector<Slot*>::const_iterator i){
    return (*i)->putItem(NULL);
}
