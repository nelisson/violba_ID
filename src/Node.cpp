#include "Node.h"

void Node::setNode(IAnimatedMeshSceneNode * node) {
    node_ = node;
}

Node::Node() {
    node_ = NULL;
}

Node::~Node() {}

