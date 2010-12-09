#ifndef ITEM_H
#define ITEM_H
#include <string>
#include "AnimatedNode.h"
#include <irrlicht/irrlicht.h>



#define DEFAULT_ITEM_LEVEL 1

using namespace std;
using namespace irr;
using namespace irr::scene;

class Item : public AnimatedNode, public ISceneNode {
    private:
        int level_;
        int ID_;
        string name_;
        static int id_;
        int generateID();

    protected:

    public:
        int getLevel();
        int getID();
        string getName();

        virtual void render() {}
        virtual const core::aabbox3d<f32>& getBoundingBox() const { }

        Item(const Item & oldItem,
             ISceneNode * parent = NULL,
             ISceneManager * manager = NULL);

        Item(ISceneNode * parent,
             ISceneManager * manager,
             string name,
             char * modelPath,
             int level = DEFAULT_ITEM_LEVEL);
        virtual ~Item();

};

#endif // ITEM_H
