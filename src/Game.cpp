#include "Game.h"

char * Game::music_[3] = {"./music/01-intro.mp3",
                         "./music/02-town.mp3",
                         "./music/03-dungeon.mp3"} ;

char * Game::sounds_[3] = {"./sounds/dead.wav",
                           "./sounds/swing.wav",
                           "./sounds/swing2.wav"} ;

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
    controller_->setCallBack(R, PRESSED, mainCharacter_->drinkPotion, mainCharacter_);
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

    delta.X = speed * elapsedTime * moveHorizontal;
    delta.Z = -1 * speed * elapsedTime * moveVertical;
    delta.Y = 0;

    thisptr->getMainCharacter()->walk(delta);
}

void Game::doActions() {
    cameras_[0]->setPosition(mainCharacter_->getPosition() + DEFAULT_CAMERA_POSITION);
    cameras_[0]->setTarget(mainCharacter_->getPosition());

    if(mainCharacter_->isAlive()) {
        mainCharacter_->refresh();

        if (mainCharacter_->tryHitCheck(sound_)) {
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

    cout << "Damage given: " << (*monster)->hurt(mainCharacter_->getDamage(), sound_) << endl;

    if (!(*monster)->isAlive()) {
        mainCharacter_->earnExperience((*monster)->getExperienceGiven());
        (*monster)->die(sound_);
        (*monster)->setState(DEAD);
        /*
        try {
            cout<<"Vo dropa."<<endl;
            Item droppedItem = itemGenerator_.dropItem(100);
            cout<<"DropedItem OK."<<endl;
            Item * item = new Item(droppedItem,
                                   level_, getSceneManager());
            
            cout<<"ItemCopy OK."<<endl;

            position2di position = (*monster)->getGridPosition();
            cout<<"PositionGridGet OK."<<endl;

            grid_.fillCell((*monster)->getGridPosition(), item);
            cout<<"CellFill OK."<<endl;
        }
        catch (exception e) {
            cout << "catch" << endl;
        }
        */
        /*
        delete (*monster);
        return --(removeMonster(monster));
        */
        return monster;
    }

    return monster;
}

void Game::attackMainCharacter(float damage) {
    cout << "Main Character damage: " << mainCharacter_->hurt(damage, sound_) <<endl;
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
        if((*monster)->isAlive()) {
            vector3df ninjaPosition = mainCharacter_->getAbsolutePosition();
            vector3df monsterPosition = (*monster)->getPosition();

            if (ninjaPosition.getDistanceFrom(monsterPosition) > (*monster)->getRange()) {

                vector3df vetor = ninjaPosition - monsterPosition;
                vetor.normalize();
                (*monster)->walk(vetor * getElapsedTime());
                if ((*monster)->getState() == STOPPING) {
                    (*monster)->setFrameLoop(MONSTER_WALK);
                    (*monster)->setState(MOVING);
                }
            }
            else{
                if((*monster)->canAttack()) {
                    attackMainCharacter((*monster)->getDamage());
                    playSound(SWING2_SOUND);
                    (*monster)->attack();

                    if ((*monster)->getState() != ATTACK_STARTING) {
                        (*monster)->setFrameLoop(MONSTER_ATTACK);
                        (*monster)->setState(ATTACK_STARTING);
                    }
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

void Game::playMusic(Music music) {
    if (musicPlaying_ != music) {
        sound_->stopAllSounds();
        sound_->play2D(music_[music], true);
        musicPlaying_ = music;
    }
}

void Game::playSound(Sound sound) {
    sound_->play2D(sounds_[sound]);
}

Game::Game(ISceneManager * sceneManager) {
    mainScreen = true;
    sceneManager_ = sceneManager;
    level_ = new Level(sceneManager);
    cout << "Level created." << endl;
    controller_ = new XBOX360Controller();
    controller_->mainScreen = &mainScreen;
    cout << "Controller created." << endl;
    mainCharacter_ = new MainCharacter(level_, sceneManager);
    cout << "Char created." << endl;
    //grid_ = Grid(level_);

    sound_ = createIrrKlangDevice();
    playMusic(TOWN);

    cout << "Grid Created." << endl;

    time(&lastSpawn_);

    lights_.push_back(getSceneManager()->addLightSceneNode());
    cameras_.push_back(getSceneManager()->addCameraSceneNode(0, DEFAULT_CAMERA_POSITION));
    cameras_[0]->setTarget(mainCharacter_->getPosition());

    setCallbacks();
    itemGenerator_.loadItems(createItems());

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
