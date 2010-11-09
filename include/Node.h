#ifndef NODE_H
#define NODE_H

#include <irrlicht/irrlicht.h>

using namespace irr::scene;

class Node {
    private:
        IAnimatedMeshSceneNode * node_;

    protected:
        Node();

    public:
        IAnimatedMeshSceneNode* getNode();

        // Wrapper node

        virtual ~Node();
};

#endif // NODE_H
