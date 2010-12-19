#include <irrlicht/ISceneNode.h>

#include "Cursor.h"

ISceneNode* Cursor::getIntersectedSceneNode(ISceneManager* manager,
                                            vector3df& collisionPoint_out) {
    ISceneCollisionManager* collision = manager->getSceneCollisionManager();

    vector2di cursorPosition = device_->getCursorControl()->getPosition();
    line3df ray = collision->getRayFromScreenCoordinates(position2di(cursorPosition.X, cursorPosition.Y));

    triangle3df triangle;

    ISceneNode* selectedNode = collision->getSceneNodeFromRayBB(ray, NodeIDFlags::ITEM);

    if (selectedNode) {
        cout << "achou node item"<<endl;
        return selectedNode;
    }
    else
        return collision->getSceneNodeAndCollisionPointFromRay(
                              ray,
                              collisionPoint_out,
                              triangle,
                              NodeIDFlags::ENEMY & NodeIDFlags::FLOOR);
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

