#include "Bar.h"
#include <iostream>

void Bar::render() {
    if (fillPercentage > 0) {
        IVideoDriver * driver = getSceneManager()->getVideoDriver();
        ISceneCollisionManager * collision = getSceneManager()->getSceneCollisionManager();

        position2di center = collision->getScreenCoordinatesFrom3DPosition(getAbsolutePosition());
        position2di upperLeft = center - position2di(width/2, height/2);
        dimension2di size(width, height);

        rect<int> outterRectangle(upperLeft, size);

        upperLeft += position2di(horizontalOffset, verticalOffset);
        size -= position2di(horizontalOffset, verticalOffset);
        rect<int> innerRectangle(upperLeft, size);

        size.Width *= fillPercentage / 100;
        rect<int> fillRectangle(upperLeft, size);

        driver->draw2DRectangleOutline(outterRectangle, borderColor);
        driver->draw2DRectangle(emptyColor, innerRectangle);
        driver->draw2DRectangle(fillRectangle, fillColor[0], fillColor[1], fillColor[0], fillColor[1]);
    }
}

void Bar::OnRegisterSceneNode() {
        if (IsVisible)
            SceneManager->registerNodeForRendering(this, ESNRP_SHADOW);

        ISceneNode::OnRegisterSceneNode();
}

Bar::Bar(ISceneNode * parent,
         ISceneManager * manager,
         float fillPercentage_,
         SColor borderColor_,
         SColor backgroundColor_,
         SColor fillColor1,
         SColor fillColor2,
         SColor emptyColor_,
         int width_,
         int height_,
         int verticalOffset_,
         int horizontalOffset_,
         int segments_,
         int segmentSeparatorWidth_)
    : ISceneNode(parent, manager, NodeIDFlags::IGNORED),
      fillPercentage(fillPercentage_),
      borderColor(borderColor_),
      backgroundColor(backgroundColor_),
      emptyColor(emptyColor_),
      width(width_),
      height(height_),
      verticalOffset(verticalOffset_),
      horizontalOffset(horizontalOffset_),
      segments(segments_),
      segmentSeparatorWidth(segmentSeparatorWidth_) {

    fillColor[0] = fillColor1;
    fillColor[1] = fillColor2;
    //getSceneManager()->setAutomaticCulling(false);
    //material_.Lighting = false;
    setAutomaticCulling(irr::scene::EAC_OFF);
}

Bar::~Bar() {
    remove();
}
