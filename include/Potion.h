#ifndef GOLD_H
#define GOLD_H
#include "Item.h"
#include "Utils.h"

#include <irrlicht/irrlicht.h>
#include <cstdlib>

#define DEFAULT_HEAL_AMOUNT 50
#define DEFAULT_POTION_MESH_PATH "./models/potion-low.3DS"
#define DEFAULT_POTION_IMAGE_PATH "./misc/potion.bmp"


using namespace irr;
using namespace irr::scene;

class Potion : public Item {
    private:
        int healAmount_;
        
    protected:

    public:
        int getHealAmount(){return healAmount_;}
        void setHealAmount(int healAmount){healAmount_ = healAmount;}

        Potion(ISceneNode * parent,
               ISceneManager * manager,
               std::string name,
               const char * modelPath = DEFAULT_POTION_MESH_PATH,
               const char * imagePath = DEFAULT_POTION_IMAGE_PATH,
               int healAmount = DEFAULT_HEAL_AMOUNT);

        virtual ~Potion();
        virtual Item* copy(ISceneNode * parent, ISceneManager * manager);
};

#endif // POTION_H
