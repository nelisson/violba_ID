#include "Character.h"

position2di Character::getGridPosition() {
    return position2di(getPosition().X, getPosition().Z);
}

float Character::heal(float value) {
    currentHP_ += value;

    if (currentHP_ > maxHP_)
        currentHP_ = maxHP_;

    healthBar_->fillPercentage = getHPPercentual();
    
    return value;
}

void Character::fillHP() {
    heal(maxHP_);
}

float Character::hurt(float value, ISoundEngine * sound) {

    currentHP_ -= value;
    if (!isAlive()) {
        currentHP_ = 0;
        die(sound);
    }

    healthBar_->fillPercentage = getHPPercentual();

    return value;
}

bool Character::isAlive() {
    return currentHP_ > 0;
}

void Character::render() {}


Character::Character(ISceneNode * parent,
                     ISceneManager * manager,
                     std::string name,
                     char * modelPath,
                     int maxHP,
                     int level,
                     float moveSpeed)
    : AnimatedNode(), ISceneNode(parent, manager) {

    cout<<"To entrando em char"<<endl;

    //vector3df center = getAnimatedNode()->getBoundingBox().getCenter();
    //size_  = dimension2df(getPosition().X + center.X, getPosition().Z + center.Z);
    name_  = name;
    level_ = level;
    maxHP_ = maxHP;
    moveSpeed_ = moveSpeed;
    healthBar_ = new Bar(this, manager);

    fillHP();
    IAnimatedMesh * mesh = getSceneManager()->getMesh(modelPath);

    cout<<"Loaded char model"<<endl;
    setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh, this));
    cout<<"Setted char node"<<endl;
    vector3df center = getAnimatedNode()->getBoundingBox().getCenter();
    cout<<"accessed animatedNode"<<endl;
    f32 x = max_(center.X, center.Z);
    setSize(dimension2df(2 * x, 2 * center.Y));
    cout<<"settedsize"<<endl;
}

Character::~Character() {
    delete healthBar_;
    remove();
}
