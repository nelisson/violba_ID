#include "Character.h"

using namespace Sounds;

position2di Character::getGridPosition() const {
    return position2di(getPosition().X, getPosition().Z);
}

recti Character::getGridRectangle() const {
    return recti(getGridPosition() - position2di(getSize().Width/2, getSize().Height/2),
                 getSize());
}

float Character::heal(float value,  bool playSound) {
    currentHP_ += value;

    if (playSound)
        playSoundEffect(POTION);

    if (currentHP_ > maxHP_)
        currentHP_ = maxHP_;

    healthBar_->fillPercentage = getHPPercentual();
    
    return value;
}

void Character::fillHP() {
    heal(maxHP_, false);
}

float Character::hurt(float value) {

    currentHP_ -= value;
    if (!isAlive()) {
        currentHP_ = 0;
        die();
    }
    else
        playSoundEffect(HURT);

    healthBar_->fillPercentage = getHPPercentual();

    return value;
}

void Character::render() {}


Character::Character(ISceneNode * parent,
                     ISceneManager * manager,
                     s32 id,
                     ISoundEngine * soundEngine,
                     vector3df offset,
                     const std::string name,
                     const char * modelPath,
                     int level)
    : AnimatedNode(), 
      ISceneNode(parent, manager, id, offset),
      SoundEmmitter(soundEngine) {

    cout<<"To entrando em char"<<endl;

    //vector3df center = getAnimatedNode()->getBoundingBox().getCenter();
    //size_  = dimension2df(getPosition().X + center.X, getPosition().Z + center.Z);
    name_  = name;
    level_ = level;
    maxHP_ = 1;
    moveSpeed_ = DEFAULT_CHARACTER_MOVESPEED;
    healthBar_ = new Bar(this, manager);

    fillHP();
    IAnimatedMesh * mesh = getSceneManager()->getMesh(modelPath);

    cout<<"Loaded char model"<<endl;
    setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh, this, getID()));
    cout<<"Setted char node"<<endl;
    vector3df center = getAnimatedNode()->getBoundingBox().getCenter();

    ITriangleSelector* selector = getSceneManager()->createOctreeTriangleSelector(mesh, getAnimatedNode());
    getAnimatedNode()->setTriangleSelector(selector);
    selector->drop();
    
    cout<<"accessed animatedNode"<<endl;
    f32 x = max_(center.X, center.Z);
    setSize(dimension2df(2 * x, 2 * center.Y));
    cout<<"settedsize"<<endl;
}

Character::~Character() {
    delete healthBar_;
    remove();
}
