#ifndef NODE_H
#define NODE_H

#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace irr::scene;

class AnimatedNode {
    private:
        IAnimatedMeshSceneNode * node_;

    protected:
        AnimatedNode();

    public:
        IAnimatedMeshSceneNode * getAnimatedNode() { return node_; };

        void setNode(IAnimatedMeshSceneNode * node);
        f32 getStartFrame() { return node_->getStartFrame(); }
        f32 getFrameNr() { return node_->getFrameNr(); }
        f32 getEndFrame() { return node_->getEndFrame(); }
        void setFrameLoop(s32 begin, s32 end) { node_->setFrameLoop(begin, end); }
        void setAnimationEndCallback (IAnimationEndCallBack *callback) { node_->setAnimationEndCallback(callback); }
        void setAnimationSpeed (f32 framesPerSecond) { node_->setAnimationSpeed(framesPerSecond); }
        void setLoopMode (bool playAnimationLooped) { node_->setLoopMode(playAnimationLooped); }
        // Wrapper node

        virtual ~AnimatedNode();
};

#endif // NODE_H
