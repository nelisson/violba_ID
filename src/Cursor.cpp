#include <irrlicht/ISceneNode.h>

#include "Cursor.h"

ISceneNode* Cursor::getIntersectedSceneNode(ISceneManager* manager) const {
    ISceneCollisionManager* collision = manager->getSceneCollisionManager();

    vector2di cursorPosition = device_->getCursorControl()->getPosition();
    line3df ray = collision->getRayFromScreenCoordinates(position2di(cursorPosition.X, cursorPosition.Y));

    vector3df collisionPoint;
    triangle3df triangle;

    ISceneNode* selectedNode = collision->getSceneNodeFromRayBB(ray, NodeIDFlags::ENEMY & NodeIDFlags::ITEM);

    if (selectedNode)
        return selectedNode;
    else
        return collision->getSceneNodeAndCollisionPointFromRay(ray, collisionPoint, triangle, NodeIDFlags::ENEMY);
}

void Cursor::render() {
    video::ITexture* cursor = driver_->getTexture(CURSOR_IMAGE_PATH);
    driver_->makeColorKeyTexture(cursor, SColor(255, 255, 255, 255));

    recti imageRect(0,0,cursor->getOriginalSize().Width, cursor->getOriginalSize().Height);
    driver_->draw2DImage(cursor, device_->getCursorControl()->getPosition(),
                        imageRect, 0, filters_[currentFilter_], true);
}

Cursor::Cursor(IrrlichtDevice* device, IVideoDriver* driver)
    : ISceneNode(0, 0, NodeIDFlags::IGNORED),
    device_(device), driver_(driver),
    currentFilter_(0) {
    
    setAutomaticCulling(irr::scene::EAC_OFF);
    filters_[0] = CURSOR_POINTING_FILTER;
    filters_[1] = CURSOR_ATTACKING_FILTER;
    filters_[2] = CURSOR_GETTING_FILTER;
}

Cursor::~Cursor() {
}

