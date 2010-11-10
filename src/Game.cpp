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

    //cout << "X: " << moveHorizontal << " Y: " << moveVertical << endl;

    float speed = thisptr->getMainCharacter()->getMoveSpeed();
    f32 elapsedTime = thisptr->getElapsedTime();

    delta.X =  speed * elapsedTime *  moveHorizontal;
    delta.Z = -1 * speed * elapsedTime *  moveVertical;

    if (thisptr->getMainCharacter()->walk(delta))
        thisptr->getCameras()[0]->setPosition(thisptr->getCameras()[0]->getPosition() + delta);
}

void Game::doActions() {
//    cout << "Checking Hits..." << endl;
    if (mainCharacter_->getState() == ATTACKING) {

        f32 startFrame = mainCharacter_->getNode()->getStartFrame();
        f32 currentFrame = mainCharacter_->getNode()->getFrameNr();
        f32 endFrame = mainCharacter_->getNode()->getEndFrame();

        if ((int)currentFrame == (int)(endFrame + startFrame)/2 )
            attackMonsters();
    }
}

vector<Monster*>::iterator Game::attackMonster(vector<Monster*>::iterator monster) {
    cout << "Dealt " << (*monster)->hurt(mainCharacter_->getDamage()) << " damage." << endl;
    if (!(*monster)->isAlive() ) {
        cout << "Monster is dead, GG." << endl;
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

            cout << "Monster In Range..." << endl;
            characterToMonster = monsterPosition - characterPosition;

            cout << "Right Attack Limit: X: " << rightAttackLimit.X;
            cout << " Y: " << rightAttackLimit.Y;
            cout << " Z: " << rightAttackLimit.Z << endl;

            cout << "Left Attack Limit: X: " << leftAttackLimit.X;
            cout << " Y: " << leftAttackLimit.Y;
            cout << " Z: " << leftAttackLimit.Z;

            cout << "Angulo: " << (180/3.14) * acos( (leftAttackLimit.dotProduct(rightAttackLimit))/ (leftAttackLimit.getLength() * rightAttackLimit.getLength()) ) << endl;

            if (rightAttackLimit.crossProduct(characterToMonster).Y > 0 &&
                leftAttackLimit.crossProduct(characterToMonster).Y < 0) {

                cout << "Attacking Monster (In angle range)..." << endl;
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
	cameras_.push_back(getSceneManager()->addCameraSceneNode(0, core::vector3df(0, 10, -30)));
}

Game::~Game() {
    delete level_;
    delete controller_;
    delete mainCharacter_;
    delete monsters_;
}
