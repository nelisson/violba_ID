#include "Game.h"

ISceneManager * Game::getSceneManager() {
    return sceneManager_;
}

Level * Game::getLevel() {
    return level_;
}

XBOX360Controller * Game::getController() {
    return controller_;
}

MainCharacter * Game::getMainCharacter() {
    return mainCharacter_;
}

vector<Monster*>& Game::getMonsters() {
    return monsters_;
}

vector<ICameraSceneNode*> Game::getCameras() {
    return cameras_;
}

vector<ILightSceneNode*> Game::getLights() {
    return lights_;
}

void Game::addMonster(Monster * monster) {
    monsters_.push_back(monster);

    IAnimatedMesh * monsterMesh = getSceneManager()->getMesh(monster->getModelPath());
    monster->setNode(getSceneManager()->addAnimatedMeshSceneNode(monsterMesh));
}

void Game::setCallbacks() {
    controller_->setCallBack(X, PRESSED, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(A, PRESSED, mainCharacter_->jump, mainCharacter_);
    controller_->setCallBack(B, PRESSED, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(Y, PRESSED, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
}

void Game::moveCharacter(void* userData, core::vector2df desl) {

    Game * thisptr = (Game*) userData;

    core::vector3df nodePosition;
    desl.normalize();
    float moveHorizontal = desl.X;
    float moveVertical = desl.Y;

    cout << "X: " << moveHorizontal << " Y: " << moveVertical << endl;

    nodePosition.X = thisptr->getMainCharacter()->getSpeed() *0.001*  moveHorizontal;
    nodePosition.Z = (-1)*thisptr->getMainCharacter()->getSpeed() *0.001*  moveVertical;

    thisptr->getMainCharacter()->walk(nodePosition);
    //thisptr->getCameras()[0]->setPosition(thisptr->getMainCharacter()->getNode()->getPosition());
}

Game::Game(ISceneManager * sceneManager) {
    sceneManager_  = sceneManager;
    level_         = new Level();
    controller_    = new XBOX360Controller();
    mainCharacter_ = new MainCharacter();

    IAnimatedMesh * mesh = getSceneManager()->getMesh(getMainCharacter()->getModelPath());
    getMainCharacter()->setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh));

    lights_.push_back(getSceneManager()->addLightSceneNode());
	cameras_.push_back(getSceneManager()->addCameraSceneNode());
	//mainCharacter_->getNode()->addChild(cameras_[0]);
    cameras_[0]->bindTargetAndRotation(true);
}

Game::~Game() {
    //dtor
}
