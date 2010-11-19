#include "Character.h"

float Character::heal(float value) {
    currentHP_ += value;

    if (currentHP_ > maxHP_)
        currentHP_ = maxHP_;

    return value;
}

void Character::fillHP() {
    currentHP_ = maxHP_;
}

float Character::hurt(float value) {

    currentHP_ -= value;
    if (currentHP_ < 0)
        currentHP_ = 0;

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
    : Node(), ISceneNode(parent, manager) {

    cout << "Created node+iscenenode..." << endl;

    name_  = name;
    level_ = level;
    maxHP_ = maxHP;
    moveSpeed_ = moveSpeed;
    healthBar_ = new Bar(this, manager);

    fillHP();
    IAnimatedMesh * mesh = getSceneManager()->getMesh(modelPath);
    cout << "Got char mesh..." << endl;
    setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh, this));
    cout << "Set anim node as child..." << endl;
}

Character::~Character() {
    delete healthBar_;
    remove();
}
