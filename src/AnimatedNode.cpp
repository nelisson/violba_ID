#include "AnimatedNode.h"

void AnimatedNode::setNode(IAnimatedMeshSceneNode * node) {
    node_ = node;
}

AnimatedNode::AnimatedNode() {
    node_ = NULL;
}

AnimatedNode::~AnimatedNode() {}

