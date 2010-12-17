#ifndef POTION_H
#define POTION_H
#include "Item.h"
#include "Utils.h"

#include <irrlicht/irrlicht.h>
#include <cstdlib>

#define DEFAULT_MAX_GOLD 50
#define DEFAULT_GOLD_MESH_PATH "./models/coin.3ds"
#define DEFAULT_GOLD_IMAGE_PATH "./misc/coin.bmp"


using namespace irr;
using namespace irr::scene;

class Gold : public Item {
    private:
        int amount_;

    protected:

    public:
        int getAmount() const {return amount_;}

        Gold(ISceneNode * parent,
               ISceneManager * manager,
               const std::string name,
               const char * modelPath = DEFAULT_GOLD_MESH_PATH,
               const char * imagePath = DEFAULT_GOLD_IMAGE_PATH,
               int amount = 0);

        virtual ~Gold();
        virtual Item* copy(ISceneNode * parent, ISceneManager * manager);
};

#endif // POTION_H