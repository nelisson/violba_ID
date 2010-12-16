#ifndef ITEM_H
#define ITEM_H
#include <string>
#include "AnimatedNode.h"
#include <irrlicht/irrlicht.h>
#include <iostream>

#define DEFAULT_ITEM_LEVEL 1
#define DROP_ANIMATION 0,39

using namespace std;
using namespace irr;
using namespace irr::scene;

class Item : public AnimatedNode, public ISceneNode {
    private:
        int level_;
        int ID_;
        string name_;
        string modelPath_;
        static int id_;

        int generateID() { return id_++; }

    protected:

    public:
        int getLevel() const { return level_; }
        int getID() const { return ID_; }
        const string getNamex() const {  return name_; }

        virtual void render() {}
        virtual const core::aabbox3d<f32>& getBoundingBox() const { }

        Item(ISceneNode * parent,
             ISceneManager * manager,
             const string name,
             const char * modelPath,
             int level = DEFAULT_ITEM_LEVEL);

        Item* copy(ISceneNode * parent = NULL,
                   ISceneManager * manager = NULL);
        
        virtual ~Item();

};

#endif // ITEM_H
