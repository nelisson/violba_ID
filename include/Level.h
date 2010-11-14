#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <iostream>
#include "Object.h"


using namespace std;
using namespace irr;
using namespace irr::scene;
using namespace irr::video;

class Level : public ISceneNode {
    private:
        vector<Object*> objects_;
        ITerrainSceneNode* terrain_;
        ISceneNode* skybox_;
        ISceneNode* skydome_;
        ITriangleSelector* selector_;

    protected:

    public:
        ITerrainSceneNode * getTerrain() { return terrain_; }
        ITriangleSelector* getTriangleSelector() { return selector_; }

        virtual void render() {}
        virtual const core::aabbox3d<f32>& getBoundingBox() const {}

        Level(ISceneManager * manager);
        virtual ~Level();
};

#endif // LEVEL_H
