#include "Game.h"

using namespace GameMusic;
using namespace State;

void Game::addMonster(Monster * monster) {
    monsters_.push_back(monster);

    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(),
                                                                              monster, core::vector3df(5, 5, 5),
                                                                              core::vector3df(0,-10.0f, 0),
                                                                              core::vector3df(0, 0, 0), 0);
    monster->addAnimator(anim);

    anim->drop();
}

vector<Monster*>::iterator Game::removeMonster(vector<Monster*>::iterator monster) {
    return monsters_.erase(monster);
}

void Game::setCallbacks() {
    controller_->setCallBack(X, PRESSED, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(A, RELEASED, mainCharacter_->jump, mainCharacter_);
    controller_->setCallBack(B, PRESSED, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(Y, PRESSED, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(R, RELEASED, mainCharacter_->drinkPotion, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
    controller_->setCallBack(L_ANALOG, RELEASED, mainCharacter_->stop, mainCharacter_);
    controller_->setCallBack(RT, PRESSED, mainCharacter_->block, mainCharacter_);
    controller_->setCallBack(RT, RELEASED, mainCharacter_->unblock, mainCharacter_);
    controller_->setCallBack(L, PRESSED, mainCharacter_->crouch, mainCharacter_);
    controller_->setCallBack(L, RELEASED, mainCharacter_->getUp, mainCharacter_);

}

void Game::moveCharacter(void* userData, vector2df desl) {
    Game * thisptr = (Game*) userData;

    vector3df delta;
    desl.normalize();
    float moveHorizontal = desl.X;
    float moveVertical = desl.Y;

    float speed = thisptr->getMainCharacter()->getMoveSpeed();
    f32 elapsedTime = thisptr->getElapsedTime();

    delta.X = speed * elapsedTime * moveHorizontal;
    delta.Z = -1 * speed * elapsedTime * moveVertical;
    delta.Y = 0;

    thisptr->getMainCharacter()->walk(delta);
}

void Game::doActions() {
    refreshSounds();
    
    if (mainCharacter_->getState() != JUMPING) {
        cameras_[0]->setTarget(mainCharacter_->getPosition());
        cameras_[0]->setPosition(mainCharacter_->getPosition() + DEFAULT_CAMERA_POSITION);
    }
    else {
        cameras_[0]->setTarget(vector3df(mainCharacter_->getPosition().X,
                                         getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                                         mainCharacter_->getPosition().Z));

        cameras_[0]->setPosition(vector3df(mainCharacter_->getPosition().X,
                                           getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                                           mainCharacter_->getPosition().Z) + DEFAULT_CAMERA_POSITION);
    }

    if(mainCharacter_->isAlive()) {
        mainCharacter_->refresh();

        if (mainCharacter_->tryHitCheck()) {
            cout << "Hits: " << attackMonsters();
        }

        time_t currentTime;
        time(&currentTime);
        if (difftime(currentTime, lastSpawn_) >= DEFAULT_MONSTER_CREATION_TIME_IN_SECONDS) {
            tryGeneratingMonster(DEFAULT_MONSTER_GENERATION_CHANCE);
            lastSpawn_ = currentTime;
        }

        runMonstersAI();
    }
    else if (mainCharacter_->getState() == DEAD) {
        sleep(1);
        mainScreen = true;
    }
}

vector<Monster*>::iterator Game::attackMonster(vector<Monster*>::iterator monster) {

    cout << "Damage given: " << (*monster)->hurt(mainCharacter_->getDamage()) << endl;

    if (!(*monster)->isAlive()) {
        mainCharacter_->earnExperience((*monster)->getExperienceGiven());
        (*monster)->die();
        (*monster)->setState(DEAD);
        
        try {
            cout<<"Vo dropa."<<endl;
            Item droppedItem = itemGenerator_.dropItem(60);
            playSoundEffect(Sounds::GOLD_DROP);
            cout<<"DropedItem OK."<<endl;

            Item * item = new Item(droppedItem,
                                   level_, getSceneManager());
            
            cout<<"ItemCopy OK."<<endl;

            grid_.fillCell((*monster)->getGridPosition(), item);
            cout<<"CellFill OK."<<endl;
        }
        catch (int i) {
            cout << "catch" << i <<  endl;
        }

        //delete (*monster);
        return --(removeMonster(monster));
        
        return monster;
    }

    return monster;
}

void Game::attackMainCharacter(float damage) {
    if (mainCharacter_->getState() != BLOCKING)
        cout << "Main Character damage: " << mainCharacter_->hurt(damage) <<endl;
    else
        mainCharacter_->playSoundEffect(Sounds::BLOCK);
}

int Game::attackMonsters() {
    int hitCounter = 0;
    vector3df characterRotation = mainCharacter_->getRotation();
    vector3df monsterPosition, characterPosition = mainCharacter_->getPosition();
    vector3df characterToMonster;

    vector3df characterForward = characterRotation.rotationToDirection();
    vector3df rightAttackLimit = vector3df(0, -mainCharacter_->getEquippedWeapon()->getAttackAngle() / 2, 0).rotationToDirection(characterForward);
    vector3df leftAttackLimit = vector3df(0, mainCharacter_->getEquippedWeapon()->getAttackAngle() / 2, 0).rotationToDirection(characterForward);

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
        Monster * newMonster = new Monster(level_, sceneManager_, getSoundEngine());
        addMonster(newMonster);

        dimension2df size = getLevel()->getSize();
        
        float randomX = randomBetween(0, size.Width);
        float randomZ = randomBetween(0, size.Height);
        float Y = getLevel()->getTerrain()->getHeight(randomX, randomZ);

        newMonster->setPosition(vector3df(randomX, Y, randomZ));
    }
}

void Game::runMonstersAI() {
    vector<Monster*>::iterator monster;
    for (monster = monsters_.begin(); monster < monsters_.end(); monster++) {
        if((*monster)->isAlive()) {
            vector3df ninjaPosition = mainCharacter_->getAbsolutePosition();
            vector3df monsterPosition = (*monster)->getPosition();

            if (ninjaPosition.getDistanceFrom(monsterPosition) > 10 * (*monster)->getRange()) {

                vector3df vetor = ninjaPosition - monsterPosition;
                vetor.normalize();
                (*monster)->walk(vetor * getElapsedTime() * 4);
                if ((*monster)->getState() == STOPPING) {
                    (*monster)->setFrameLoop(MONSTER_RUN);
                    (*monster)->setLoopMode(true);
                    (*monster)->setState(RUNNING);
                }
            }

            else if (ninjaPosition.getDistanceFrom(monsterPosition) > (*monster)->getRange()) {

                vector3df vetor = ninjaPosition - monsterPosition;
                vetor.normalize();
                (*monster)->walk(vetor * getElapsedTime());
                if ((*monster)->getState() == RUNNING) {
                    (*monster)->setFrameLoop(MONSTER_WALK);
                    (*monster)->setLoopMode(true);
                    (*monster)->setState(MOVING);
                }
            }
            else{
                if((*monster)->canAttack()) {
                    attackMainCharacter((*monster)->getDamage());
                    (*monster)->playSoundEffect(Sounds::SWING1);
                    (*monster)->attack();
                    (*monster)->setFrameLoop(MONSTER_ATTACK);
                    (*monster)->setLoopMode(false);
                }
            }
        }
        else
            if ((*monster)->getState() == DEAD || (*monster)->getState() == DYING  ) {
                delete (*monster);
                removeMonster(monster);
            }
    }
}

vector<Weapon> Game::loadWeapons(){
    vector<Weapon> weapons;

    weapons.push_back(Weapon(NULL, NULL, "Arma 1"));
    weapons.push_back(Weapon(NULL, NULL, "Arma 2"));
    weapons.push_back(Weapon(NULL, NULL, "Arma 3"));
    weapons.push_back(Weapon(NULL, NULL, "Arma 4"));
    
    return weapons;
}

vector<Armor> Game::loadArmors(){
    vector<Armor> armors;
    
    armors.push_back(Armor(NULL, NULL, "Armor 1"));
    armors.push_back(Armor(NULL, NULL, "Armor 2"));
    armors.push_back(Armor(NULL, NULL, "Armor 3"));

    return armors;
}

vector<Item> Game::createItems() {
    vector<Item> result;

    vector<Weapon> weapons = loadWeapons();
    vector<Armor> armors = loadArmors();

    result.insert(result.end(), weapons.begin(), weapons.end());
    result.insert(result.end(), armors.begin(), armors.end());

    return result;
}

Game::Game(ISceneManager * sceneManager, ISoundEngine * soundEngine)
    : SoundEmmitter(soundEngine) {
    mainScreen = true;
    sceneManager_ = sceneManager;
    level_ = new Level(sceneManager);
    cout << "Level created." << endl;
    controller_ = new XBOX360Controller();
    controller_->mainScreen = &mainScreen;
    cout << "Controller created." << endl;

    dimension2df terrainSize = getLevel()->getSize();
    float levelHeight = getLevel()->getTerrain()->getHeight(terrainSize.Width/2, terrainSize.Height/2);

    vector3df levelCenter(terrainSize.Width / 2, levelHeight, terrainSize.Height / 2);

    mainCharacter_ = new MainCharacter(level_, sceneManager, soundEngine, levelCenter);
    cout << "Char created." << endl;
    grid_ = Grid(level_);

    addMusic("./music/01-intro.mp3");
    addMusic("./music/02-town.mp3");
    addMusic("./music/03-dungeon.mp3");

    addSoundEffect("./sounds/itemDrop.wav");
    addSoundEffect("./sounds/selectItem.wav");
    addSoundEffect("./sounds/goldDrop.wav");
    cout << "Loaded game Music" <<endl;

   // playMusic(TOWN);

    cout << "Grid Created." << endl;

    time(&lastSpawn_);

    lights_.push_back(getSceneManager()->addLightSceneNode());
    cameras_.push_back(getSceneManager()->addCameraSceneNode(level_, DEFAULT_CAMERA_POSITION));
    cameras_[0]->setTarget(mainCharacter_->getPosition());

    setCallbacks();
    itemGenerator_.loadItems(createItems());

    sceneManager_->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(),
                                                                              mainCharacter_,
                                                                              vector3df(5, 5, 5),
                                                                              core::vector3df(0,-2.0f, 0),
                                                                              core::vector3df(0, 0, 0), 0);

    mainCharacter_->addAnimator(anim);

    anim->drop();
}

Game::~Game() {
    //delete level_;
    delete controller_;
    delete mainCharacter_;
}
