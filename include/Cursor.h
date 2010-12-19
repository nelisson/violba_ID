#ifndef CURSOR_H
#define	CURSOR_H

#include <irrlicht/irrlicht.h>
#include <iostream>
#include "Utils.h"

#define CURSOR_IMAGE_PATH "./misc/cursor.bmp"

#define CURSOR_POINTING_FILTER SColor(255, 255, 255, 255)
#define CURSOR_ATTACKING_FILTER SColor(255, 255, 0, 0)
#define CURSOR_GETTING_FILTER SColor(255, 255, 255, 0)

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

using namespace std;

namespace CursorColors {
    enum CursorColors {
        POINTING,
        ATTACKING,
        GETTING,
    };
}

class Cursor : public ISceneNode {

    private:
        IrrlichtDevice* device_;
        IVideoDriver* driver_;
        SColor filters_[3];
        int currentFilter_;

    public:
        void setFilter(int filter) { currentFilter_ = filter; }
        ISceneNode* getIntersectedSceneNode(ISceneManager* manager) const;

        virtual void render();
        virtual const core::aabbox3d<f32>& getBoundingBox() const {};

        Cursor(IrrlichtDevice* device, IVideoDriver* driver);
        virtual ~Cursor();
};

#endif	/* CURSOR_H */