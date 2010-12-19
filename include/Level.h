#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <iostream>
#include "Object.h"
#include "micropather.h"
#include "Utils.h"

using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace micropather;

class Level : public ISceneNode, public Graph {
    private:
        vector<Object*> objects_;
        ITerrainSceneNode* terrain_;
        ISceneNode* skybox_;
        ISceneNode* skydome_;
        ITriangleSelector* selector_;
        dimension2df size_;

        vector< vector<float> > matrix_;

    protected:


    public:
        ITerrainSceneNode * getTerrain() { return terrain_; }
        ITriangleSelector* getTriangleSelector() { return selector_; }
        
        dimension2df getSize() { return size_; }

        virtual void render() {}
        virtual const core::aabbox3d<f32>& getBoundingBox() const {}

        virtual float LeastCostEstimate(void* stateStart, void* stateEnd);
        virtual void AdjacentCost(void* state, vector<StateCost>* adjacent);
        virtual void PrintStateInfo(void* state) {}

        Level(ISceneManager * manager);
        virtual ~Level();
};

#endif // LEVEL_H
