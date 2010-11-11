#include "Node.h"

IAnimatedMeshSceneNode* Node::getNode() {
    return node_;
}

void Node::setNode(IAnimatedMeshSceneNode * node) {
    node_ = node;
}

Node::Node() {
    node_ = NULL;
    //ctor
}

Node::~Node() {
    if (node_ != NULL)
        node_->remove();
}
