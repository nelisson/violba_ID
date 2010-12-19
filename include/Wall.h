#ifndef WALL_H
#define WALL_H
#include <string>
#include "AnimatedNode.h"
#include "Utils.h"
#include <irrlicht/irrlicht.h>
#include <iostream>

#define DEFAULT_TEXTURE_PATH "./models/Wall1.tga"

using namespace std;
using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;


class Wall : IBillboardSceneNode {
    private:
                
    protected:
        
    public:
        Wall(ISceneNode *parent = 0,
             const core::dimension2d< f32 > &size = core::dimension2d< f32 >(10.0f, 10.0f),
             const core::vector3df &position = core::vector3df(0, 0, 0),
             s32 id = NodeIDFlags::IGNORED,
             video::SColor colorTop = 0xFFFFFFFF,
             video::SColor colorBottom = 0xFFFFFFFF);

};

#endif //WALL_H
