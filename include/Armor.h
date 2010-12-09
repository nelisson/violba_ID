#ifndef ARMOR_H
#define ARMOR_H

#define DEFAULT_ARMOR_MESH_PATH "./models/sword.x"

#include <irrlicht/irrlicht.h>
#include "Item.h"

enum weightClass{light, normal, heavy};

using namespace irr;
using namespace irr::scene;

class Armor : public Item {
    private:
        int amount;
        weightClass type;

    protected:

    public:
        Armor(ISceneNode * parent,
              ISceneManager * manager,
              std::string name,
              char * modelPath = DEFAULT_ARMOR_MESH_PATH);

        virtual ~Armor();
};

#endif // ARMOR_H
