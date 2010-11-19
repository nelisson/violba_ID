#include "Game.h"

void Game::addMonster(Monster * monster) {
    monsters_.push_back(monster);

	ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(), monster, core::vector3df(5, 5, 5));
	monster->addAnimator(anim);

	anim->drop();
}

vector<Monster*>::iterator Game::removeMonster(vector<Monster*>::iterator monster) {
    return monsters_.erase(monster);
}


void Game::setCallbacks() {
    controller_->setCallBack(X, PRESSED, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(A, PRESSED, mainCharacter_->jump, mainCharacter_);
    controller_->setCallBack(B, PRESSED, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(Y, PRESSED, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
    controller_->setCallBack(L_ANALOG, RELEASED, mainCharacter_->stop, mainCharacter_);
}

void Game::moveCharacter(void* userData, vector2df desl) {
    Game * thisptr = (Game*) userData;

    vector3df delta;
    desl.normalize();
    float moveHorizontal = desl.X;
    float moveVertical = desl.Y;

    float speed = thisptr->getMainCharacter()->getMoveSpeed();
    f32 elapsedTime = thisptr->getElapsedTime();

    delta.X =  speed * elapsedTime *  moveHorizontal;
    delta.Z = -1 * speed * elapsedTime *  moveVertical;
    delta.Y = 0;

    thisptr->getMainCharacter()->walk(delta);
}

void Game::doActions() {
    cameras_[0]->setPosition(mainCharacter_->getPosition() + DEFAULT_CAMERA_POSITION);
    cameras_[0]->setTarget(mainCharacter_->getPosition());

    mainCharacter_->refresh();

    if (mainCharacter_->tryHitCheck())
        cout << "Hits: " << attackMonsters();

    tryGeneratingMonster(DEFAULT_MONSTER_GENERATION_CHANCE_PER_FRAME);
    runMonstersAI();
}

vector<Monster*>::iterator Game::attackMonster(vector<Monster*>::iterator monster) {

    cout << "Damage given: " << (*monster)->hurt(mainCharacter_->getDamage()) << endl;

    if (!(*monster)->isAlive() ) {
        mainCharacter_->earnExperience((*monster)->getExperienceGiven());
        delete (*monster);
        return --(removeMonster(monster));
    }

    return monster;
}

int Game::attackMonsters() {
    int hitCounter = 0;
    vector3df characterRotation = mainCharacter_->getRotation();
    vector3df monsterPosition, characterPosition = mainCharacter_->getPosition();
    vector3df characterToMonster;

    vector3df characterForward = characterRotation.rotationToDirection();
    vector3df rightAttackLimit = vector3df(0, - mainCharacter_->getEquippedWeapon()->getAttackAngle()/2, 0).rotationToDirection(characterForward);
    vector3df leftAttackLimit = vector3df(0, mainCharacter_->getEquippedWeapon()->getAttackAngle()/2, 0).rotationToDirection(characterForward);

    vector<Monster*>::iterator monster;
    for (monster = monsters_.begin(); monster < monsters_.end(); monster++) {

        monsterPosition = (*monster)->getAbsolutePosition();
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

void Game::tryGeneratingMonster(int chancePercent) {
    if (randomBetween(0, 100) <= chancePercent) {
        Monster * newMonster = new Monster(level_, sceneManager_, "Dwarf da morte", "./models/dwarf.x");
        addMonster(newMonster);
        newMonster->setPosition(vector3df(randomBetween(-150, 150),
                                          0.0,
                                          randomBetween(-150, 150)));
    }
}

void Game::runMonstersAI() {
    vector<Monster*>::iterator monster;
    for (monster = monsters_.begin(); monster < monsters_.end(); monster++) {
        vector3df ninjaPosition = mainCharacter_->getAbsolutePosition();
        vector3df monsterPosition = (*monster)->getPosition();

        if (ninjaPosition.getDistanceFrom(monsterPosition) > (*monster)->getRange()) {

            vector3df vetor = ninjaPosition - monsterPosition;
            vetor.normalize();
            (*monster)->walk(vetor * getElapsedTime());
        }
    }
}

Game::Game(ISceneManager * sceneManager) {
    sceneManager_  = sceneManager;
    level_         = new Level(sceneManager);
    controller_    = new XBOX360Controller();
    mainCharacter_ = new MainCharacter(level_, sceneManager);

    lights_.push_back(getSceneManager()->addLightSceneNode());
	cameras_.push_back(getSceneManager()->addCameraSceneNode(0, DEFAULT_CAMERA_POSITION));
	cameras_[0]->setTarget(mainCharacter_->getPosition());

    setCallbacks();

    sceneManager_->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(), mainCharacter_, vector3df(5, 5, 5));
	mainCharacter_->addAnimator(anim);

	anim->drop();
}

Game::~Game() {
    //delete level_;
    delete controller_;
    delete mainCharacter_;
}
