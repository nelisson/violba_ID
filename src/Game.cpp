#include "Game.h"

ISceneManager * Game::getSceneManager() {
    return sceneManager_;
}

Level& Game::getLevel() {
    return level_;
}

XBOX360Controller& Game::getController() {
    return controller_;
}

MainCharacter& Game::getMainCharacter() {
    return mainCharacter_;
}

vector<Monster>& Game::getMonsters() {
    return monsters_;
}

vector<ICameraSceneNode*> Game::getCameras() {
    return cameras_;
}

vector<ILightSceneNode*> Game::getLights() {
    return lights_;
}

void Game::addMonster(Monster& monster) {
    monsters_.push_back(monster);

    IAnimatedMesh * monsterMesh = getSceneManager()->getMesh(monster.getModelPath());
    getSceneManager()->addAnimatedMeshSceneNode(monsterMesh);
}

Game::Game(ISceneManager * sceneManager) {
    sceneManager_ = sceneManager;

    IAnimatedMesh * mesh = getSceneManager()->getMesh(getMainCharacter().getModelPath());
    getMainCharacter().setNode(getSceneManager()->addAnimatedMeshSceneNode(mesh));

    lights_.push_back(getSceneManager()->addLightSceneNode());
	cameras_.push_back(getSceneManager()->addCameraSceneNode());
}

Game::~Game() {
    //dtor
}
