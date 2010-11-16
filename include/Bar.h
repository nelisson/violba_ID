#ifndef BAR_H
#define BAR_H

#include <irrlicht/irrlicht.h>

#define DEFAULT_BAR_FILL_PERCENTAGE 20

#define DEFAULT_BAR_BORDER_COLOR     SColor(255, 0, 255, 255)
#define DEFAULT_BAR_BACKGROUND_COLOR SColor(255, 0, 0, 0)
#define DEFAULT_BAR_FILL_COLOR       SColor(255, 0, 255, 0)
#define DEFAULT_BAR_FILL_COLOR2      SColor(255, 0, 100, 0)
#define DEFAULT_BAR_EMPTY_COLOR      SColor(255, 255, 0, 0)

#define DEFAULT_BAR_WIDTH  60
#define DEFAULT_BAR_HEIGHT 7

#define DEFAULT_BAR_VERTICAL_OFFSET   1
#define DEFAULT_BAR_HORIZONTAL_OFFSET 2

#define DEFAULT_BAR_SEGMENTS                1
#define DEFAULT_BAR_SEGMENT_SEPARATOR_WIDTH 1

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

enum BarColor {
    COLOR1, COLOR2, COLOR_COUNT = 2,
};

class Bar : public ISceneNode {
    private:
        SMaterial material_;

    protected:

    public:
        float fillPercentage;

        SColor borderColor;
        SColor backgroundColor;
        SColor fillColor[COLOR_COUNT];
        SColor emptyColor;

        int width;
        int height;
        int verticalOffset;
        int horizontalOffset;

        int segments;
        int segmentSeparatorWidth;

        virtual void render();
        virtual const core::aabbox3d<f32>& getBoundingBox() const {};
        virtual void OnRegisterSceneNode();

        Bar(ISceneNode * parent,
            ISceneManager * manager,
            float fillPercentage_ = DEFAULT_BAR_FILL_PERCENTAGE,
            SColor borderColor = DEFAULT_BAR_BORDER_COLOR,
            SColor backgroundColor_ = DEFAULT_BAR_BACKGROUND_COLOR,
            SColor fillColor1 = DEFAULT_BAR_FILL_COLOR,
            SColor fillColor2 = DEFAULT_BAR_FILL_COLOR2,
            SColor emptyColor_ = DEFAULT_BAR_EMPTY_COLOR,
            int width_ = DEFAULT_BAR_WIDTH,
            int height_ = DEFAULT_BAR_HEIGHT,
            int verticalOffset_ = DEFAULT_BAR_VERTICAL_OFFSET,
            int horizontalOffset_ = DEFAULT_BAR_HORIZONTAL_OFFSET,
            int segments_ = DEFAULT_BAR_SEGMENTS,
            int segmentsSeparatorWidth_ = DEFAULT_BAR_SEGMENT_SEPARATOR_WIDTH);

        virtual ~Bar();
};

#endif // BAR_H
