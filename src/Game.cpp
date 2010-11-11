#include "Game.h"

void Game::addMonster(Monster * monster) {
    monsters_->push_back(monster);

    IAnimatedMesh * monsterMesh = getSceneManager()->getMesh(monster->getModelPath());
    monster->setNode(getSceneManager()->addAnimatedMeshSceneNode(monsterMesh));
}

void Game::setCallbacks() {
    controller_->setCallBack(X, PRESSED, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(A, PRESSED, mainCharacter_->jump, mainCharacter_);
    controller_->setCallBack(B, PRESSED, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(Y, PRESSED, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
    controller_->setCallBack(L_ANALOG, RELEASED, mainCharacter_->stop, mainCharacter_);
}

void Game::moveCharacter(void* userData, core::vector2df desl) {
    Game * thisptr = (Game*) userData;

    core::vector3df delta;
    desl.normalize();
    float moveHorizontal = desl.X;
    float moveVertical = desl.Y;

    float speed = thisptr->getMainCharacter()->getMoveSpeed();
    f32 elapsedTime = thisptr->getElapsedTime();

    delta.X =  speed * elapsedTime *  moveHorizontal;
    delta.Z = -1 * speed * elapsedTime *  moveVertical;

    if (thisptr->getMainCharacter()->walk(delta)) {
        thisptr->getCameras()[0]->setPosition(thisptr->getCameras()[0]->getPosition() + delta);
        thisptr->getCameras()[0]->setTarget(thisptr->getMainCharacter()->getNode()->getPosition());
    }
}

void Game::doActions() {
    if (mainCharacter_->getState() == ATTACKING) {

        f32 startFrame = mainCharacter_->getNode()->getStartFrame();
        f32 currentFrame = mainCharacter_->getNode()->getFrameNr();
        f32 endFrame = mainCharacter_->getNode()->getEndFrame();

        if ((int)currentFrame == (int)(endFrame + startFrame)/2 )
            attackMonsters();
    }
}

vector<Monster*>::iterator Game::attackMonster(vector<Monster*>::iterator monster) {
    (*monster)->hurt(mainCharacter_->getDamage());
    if (!(*monster)->isAlive() ) {
        delete (*monster);
        return --(getMonsters()->erase(monster));
    }
    return monster;
}

int Game::attackMonsters() {
    int hitCounter = 0;
    vector3df characterRotation = mainCharacter_->getNode()->getRotation();
    vector3df monsterPosition, characterPosition = mainCharacter_->getNode()->getAbsolutePosition();
    vector3df characterToMonster;

    vector3df characterForward = characterRotation.rotationToDirection();
    vector3df rightAttackLimit = vector3df(0, - mainCharacter_->getEquippedWeapon()->getAttackAngle(), 0).rotationToDirection(characterForward);
    vector3df leftAttackLimit = vector3df(0, mainCharacter_->getEquippedWeapon()->getAttackAngle(), 0).rotationToDirection(characterForward);

    vector<Monster*>::iterator monster;
    for (monster = getMonsters()->begin(); monster < getMonsters()->end(); monster++) {

        monsterPosition = (*monster)->getNode()->getAbsolutePosition();

        if (monsterPosition.getDistanceFrom(characterPosition) <= mainCharacter_->getEquippedWeapon()->getRange()) {
            characterToMonster = monsterPosition - characterPosition;

            if (rightAttackLimit.crossProduct(characterToMonster).Y > 0 &&
                leftAttackLimit.crossProduct(characterToMonster).Y < 0) {
                monster = attackMonster(monster);
                hitCounter++;
            }
        }
    }

    return hitCounter;
}

Game::Game(ISceneManager * sceneManager) {
    sceneManager_  = sceneManager;
    level_         = new Level();
    controller_    = new XBOX360Controller();
    mainCharacter_ = new MainCharacter();
    monsters_      = new vector<Monster*>();

    IAnimatedMesh * mesh = getSceneManager()->getMesh(getMainCharacter()->getModelPath());
    getMainCharacter()->setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh));

    lights_.push_back(getSceneManager()->addLightSceneNode());
	cameras_.push_back(getSceneManager()->addCameraSceneNode(0, core::vector3df(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z)));
}

Game::~Game() {
    delete level_;
    delete controller_;
   // delete mainCharacter_;
    delete monsters_;
}
