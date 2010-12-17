#ifndef ARMOR_H
#define ARMOR_H

#define DEFAULT_ARMOR_MESH_PATH "./models/helmet.3DS"
#define DEFAULT_ARMOR_IMAGE_PATH "./misc/helmet.png"

#include <irrlicht/irrlicht.h>
#include "Item.h"

enum weightClass{light, normal, heavy};

using namespace irr;
using namespace irr::scene;

class Armor : public Item {
    private:
        int amount_;
        weightClass type_;

    protected:

    public:
        Armor(ISceneNode* parent,
              ISceneManager* manager,
              const std::string name,
              const char* modelPath = DEFAULT_ARMOR_MESH_PATH,
              const char* imagePath = DEFAULT_ARMOR_IMAGE_PATH);

        virtual ~Armor();
};

#endif // ARMOR_H
