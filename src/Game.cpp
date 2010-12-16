#include "Game.h"

using namespace GameMusic;
using namespace GameFonts;
using namespace State;

void Game::addMonster(Monster * monster) {
    monsters_.push_back(monster);

    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(),
            monster, core::vector3df(5, 5, 5),
            core::vector3df(0, -10.0f, 0),
            core::vector3df(0, 0, 0), 0);
    monster->addAnimator(anim);

    anim->drop();
}

vector<Monster*>::iterator Game::removeMonster(vector<Monster*>::iterator monster) {
    return monsters_.erase(monster);
}

void Game::setCallbacks() {
    controller_->setCallBack(X, PRESSED, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(X, HOLD, mainCharacter_->slash, mainCharacter_);
    controller_->setCallBack(A, PRESSED, mainCharacter_->jump, mainCharacter_);
    controller_->setCallBack(B, PRESSED, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(B, HOLD, mainCharacter_->spin, mainCharacter_);
    controller_->setCallBack(Y, PRESSED, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(Y, HOLD, mainCharacter_->kick, mainCharacter_);
    controller_->setCallBack(L_ANALOG_BUTTON, PRESSED, mainCharacter_->drinkPotion, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
    controller_->setCallBack(L_ANALOG, RELEASED, mainCharacter_->stop, mainCharacter_);
    controller_->setCallBack(L, PRESSED, mainCharacter_->crouch, mainCharacter_);
    controller_->setCallBack(L, RELEASED, mainCharacter_->getUp, mainCharacter_);
    controller_->setCallBack(R, PRESSED, mainCharacter_->block, mainCharacter_);
    controller_->setCallBack(R, HOLD, mainCharacter_->block, mainCharacter_);
    controller_->setCallBack(R, RELEASED, mainCharacter_->unblock, mainCharacter_);

    controller_->setCallBack(START, PRESSED, this->showStatus, this);
    controller_->setCallBack(START, RELEASED, this->hideStatus, this);

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

bool Game::doActions() {
    refreshSounds();

    if(isStatusVisible_){
        sceneManager_->getVideoDriver()->draw2DRectangle(SColor(255, 120, 120, 120),rect<s32> (0, 0,1024, 683));
        sceneManager_->getGUIEnvironment()->drawAll();
        return true;
    }
    if (mainScreen_) {
        sceneManager_->getGUIEnvironment()->drawAll();
        return isRunning_;
    }

    if (needsRestart_) {
        sceneManager_->getGUIEnvironment()->clear();
        load();
        needsRestart_ = false;
    }

    clearCorpses();

    getSceneManager()->drawAll();

    if (mainCharacter_->getState() != JUMPING) {
        cameras_[0]->setTarget(mainCharacter_->getPosition());
        cameras_[0]->setPosition(mainCharacter_->getPosition() + DEFAULT_CAMERA_POSITION);
    } else {
        cameras_[0]->setTarget(vector3df(mainCharacter_->getPosition().X,
                getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                mainCharacter_->getPosition().Z));

        cameras_[0]->setPosition(vector3df(mainCharacter_->getPosition().X,
                getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                mainCharacter_->getPosition().Z) + DEFAULT_CAMERA_POSITION);
    }


    if (mainCharacter_->isAlive()) {
        mainCharacter_->refresh();

        if (mainCharacter_->tryHitCheck()) {
            cout << "Hits: " << attackMonsters();
        }

        if (mainCharacter_->getState() == GETTING_ITEM) {

            try {
                vector<Item*> items = grid_.getItems(mainCharacter_->getGridRectangle());
                vector<Item*> pickedItems;

                int counter = 0;
                vector<Item*>::iterator i;
                for (i < items.begin(); i < items.end(); i++) {
                    cout << "Entrando no for." <<endl;
                    try {
                        cout << "Vo coloca item." << endl;
                        printf("%p", mainCharacter_->getInventory()->removeItem(0));
                        cout<<endl;
                        mainCharacter_->getInventory()->putItem(*i);
                        cout << "Vo coloquei item." <<endl;

                        counter++;
                        pickedItems.push_back(*i);
                        (*i)->setVisible(false);
                    }
                    catch(int j) {
                        cout << "Inventory is full." <<endl;
                    }
                }

                cout << "Got " << counter << " items."<<endl;
            }
            catch (int i) {}
            
            mainCharacter_->setState(CROUCHING);
        }


        time_t currentTime;
        time(&currentTime);
        if (difftime(currentTime, lastSpawn_) >= DEFAULT_MONSTER_CREATION_TIME_IN_SECONDS) {
            tryGeneratingMonster(DEFAULT_MONSTER_GENERATION_CHANCE);
            lastSpawn_ = currentTime;
        }

        runMonstersAI();
    } else if (mainCharacter_->getState() == DEAD) {
        sleep(1);
        mainScreen_ = true;
        createMainScreen();
    }

    return isRunning_;
}

void Game::clearCorpses() {
    vector<Monster*>::iterator i;
    for (i = monsters_.begin(); i < monsters_.end(); i++)
        if ( (*i)->getState() == DEAD) {
            (*i)->decreaseCorspeDelay(elapsedTime_);

            if ( (*i)->getCorspeDelay() < 0) {
                delete (*i);
                i = monsters_.erase(i)--;
            }
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
            Item droppedItem = itemGenerator_.dropItem(DEFAULT_ITEM_GENERATION_CHANCE);
            playSoundEffect(Sounds::GOLD_DROP);
            
            cout<<"DropedItem OK." << endl;

            Item * item = droppedItem.copy(level_, getSceneManager());
            
            cout<<"ItemCopy OK."<<endl;

            vector3df position = vector3df((*monster)->getGridPosition().X,
                                            0,
                                           (*monster)->getGridPosition().Y);
            
            position.Y = getLevel()->getTerrain()->getHeight(position.X, position.Z);
            item->setPosition(position);
//            item->setLoopMode(false);
            item->setFrameLoop(DROP_ANIMATION);
            item->setAnimationSpeed(20);
            

            grid_.fillCell((*monster)->getGridPosition(), item);
            cout << "CellFill OK." << endl;
        } catch (int i) {
            cout << "catch" << i << endl;
        }

        //delete (*monster);

        return monster;
    }

    return monster;
}

void Game::attackMainCharacter(float damage) {
    if (mainCharacter_->getState() != BLOCKING)
        cout << "Main Character damage: " << mainCharacter_->hurt(damage) << endl;
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

        if ( (*monster)->isAlive() ) {

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
    }

    return hitCounter;
}

void Game::tryGeneratingMonster(int chancePercent) {
    if (randomBetween(0, 100) <= chancePercent && monsters_.size() < MAX_MONSTERS) {
        Monster * newMonster = new Monster(level_, sceneManager_, getSoundEngine());
        addMonster(newMonster);

        dimension2df size = getLevel()->getSize();

        float randomX = randomBetween(0, size.Width);
        float randomZ = randomBetween(0, size.Height);
        float Y = 1 + getLevel()->getTerrain()->getHeight(randomX, randomZ);

        newMonster->setPosition(vector3df(randomX, Y, randomZ));
    }
}

void Game::runMonstersAI() {
    vector<Monster*>::iterator monster;
    for (monster = monsters_.begin(); monster < monsters_.end(); monster++) {
        if ((*monster)->isAlive()) {
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
            } else if (ninjaPosition.getDistanceFrom(monsterPosition) > (*monster)->getRange()) {

                vector3df vetor = ninjaPosition - monsterPosition;
                vetor.normalize();
                (*monster)->walk(vetor * getElapsedTime());
                if ((*monster)->getState() == RUNNING) {
                    (*monster)->setFrameLoop(MONSTER_WALK);
                    (*monster)->setLoopMode(true);
                    (*monster)->setState(MOVING);
                }
            } else {
                if ((*monster)->canAttack()) {
                    attackMainCharacter((*monster)->getDamage());
                    (*monster)->playSoundEffect(Sounds::SWING1);
                    (*monster)->attack();
                    (*monster)->setFrameLoop(MONSTER_ATTACK);
                    (*monster)->setLoopMode(false);
                }
            }
        }
    }
}

void Game::createMainScreen() {
    IGUIEnvironment* env = sceneManager_->getGUIEnvironment();
    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("./misc/diablo28.xml");

    int deslocX = 200, deslocY = 50;
    int x0 = 70, y0 = 520, y1 = y0 + 60;

    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
    env->addImage(sceneManager_->getVideoDriver()->getTexture("./models/mainScreen.png"),
            position2d<int>(0, 0));
    env->addButton(rect<s32 > (x0, y0, x0 + deslocX, y0 + deslocY), 0, GUI_ID_PLAY_DEMO_BUTTON,
            L"Play Demo");
    env->addButton(rect<s32 > (x0, y1, x0 + deslocX, y1 + deslocY), 0, GUI_ID_QUIT_BUTTON,
            L"Quit");

    cout<<"VO playa"<<endl;
    playMusic(GameMusic::TOWN, true, true, 2);
}

bool Game::OnEvent(const SEvent& event) {

    controller_->OnEvent(event);

    if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
            switch (id) {
                case GUI_ID_QUIT_BUTTON:
                    playSoundEffect(Sounds::SELECTION);
                    isRunning_ = false;

                    break;

                case GUI_ID_PLAY_DEMO_BUTTON:
                    playSoundEffect(Sounds::SELECTION);
                    mainScreen_ = false;
                    needsRestart_ = true;
                    break;
            }

        return true;
    } else
        return false;
}

void Game::createStatusSreen() {
    IGUIEnvironment* env = sceneManager_->getGUIEnvironment();
    IGUIStaticText* text;
    const char* Temp;
    char* num = (char*)malloc(15);
    rect<s32> tamanho;
    position2di position;
    dimension2d<u32> screen = RESOLUTION_SCREEN;

    Temp="Status";
    tamanho = getStringSize(Temp, 48);
    position = position2di((screen.Width - tamanho.getWidth())/2,0);
    text = env->addStaticText(L"Status",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO48));
    text->setRelativePosition(position);

    Temp="HP:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,screen.Height/6 - tamanho.getHeight()/2);
    text = env->addStaticText(L"HP:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%.0f/",mainCharacter_->getCurrentHP());
    position += position2di(tamanho.getWidth(),0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getCurrentHP(),true),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%.0f",mainCharacter_->getMaxHP());
    position += position2di(tamanho.getWidth()*(float)3/4,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getMaxHP()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp="Level:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,(screen.Height/6)*1.25 - tamanho.getHeight()/2);
    text = env->addStaticText(L"Level:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%d",mainCharacter_->getLevel());
    position += position2di(tamanho.getWidth()/1.5,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getLevel()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp="Vitality:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,(screen.Height/6)*1.5 - tamanho.getHeight()/2);
    position += position2di(0,30);
    text = env->addStaticText(L"Vitality:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%d",mainCharacter_->getVitality());
    position += position2di(tamanho.getWidth()/1.5,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getVitality()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp="Agility:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,(screen.Height/6)*1.75 - tamanho.getHeight()/2);
    position += position2di(0,30);
    text = env->addStaticText(L"Agility:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%d",mainCharacter_->getAgility());
    position += position2di(tamanho.getWidth()/1.5,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getAgility()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp="Strength:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,(screen.Height/6)*2 - tamanho.getHeight()/2);
    position += position2di(0,30);
    text = env->addStaticText(L"Strength:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%d",mainCharacter_->getStrength());
    position += position2di(tamanho.getWidth()/1.5,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getStrength()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);
    
    Temp="Experience:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30,(screen.Height/6)*2.25 - tamanho.getHeight()/2);
    position += position2di(0,30);
    text = env->addStaticText(L"Experience:",tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num,"%d",mainCharacter_->getCurrentExperience());
    position += position2di(tamanho.getWidth()/1.5,0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getCurrentExperience()),tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    //exp -> current nextLevel
    //jumpHeight
    //moveSpeed
    //AttackSpeed
    //Position
 }

void Game::showStatus(void *userData) {
    Game * thisptr = (Game*) userData;

    if(!thisptr->mainScreen_){
        thisptr->isStatusVisible_ = true;
        thisptr->createStatusSreen();
        thisptr->getSceneManager()->getGUIEnvironment()->drawAll();
    }
}

void Game::hideStatus(void *userData) {
    Game * thisptr = (Game*) userData;

    if(!thisptr->mainScreen_) {
        thisptr->isStatusVisible_ = false;
        thisptr->getSceneManager()->getGUIEnvironment()->clear();
    }
}

void Game::load() {
    dimension2df terrainSize = getLevel()->getSize();
    float levelHeight = 1 + getLevel()->getTerrain()->getHeight(terrainSize.Width / 2,
                                                            terrainSize.Height / 2);

    vector3df levelCenter(terrainSize.Width / 2, levelHeight, terrainSize.Height / 2);
    mainCharacter_->setPosition(levelCenter);
    mainCharacter_->fillHP();
    mainCharacter_->setLevel(1);
    mainCharacter_->setState(STOPPING);

    grid_.clear();
    cout << "Grid cleared." << endl;

    vector<Monster*>::iterator i;
    for (i = monsters_.begin(); i < monsters_.end(); i++) {
        delete (*i);
        i = monsters_.erase(i)--;
    }

    cout << "Monsters deleted." << endl;

    time(&lastSpawn_);

    cameras_[0]->setTarget(mainCharacter_->getPosition());

    setCallbacks();
    playMusic(GameMusic::DUNGEON, true);
}

Game::Game(ISceneManager * sceneManager, ISoundEngine * soundEngine)
    : SoundEmmitter(soundEngine),
      isStatusVisible_(false),
      isRunning_(true),
      mainScreen_(true),
      sceneManager_(sceneManager) {

    IGUIEnvironment* env = sceneManager_->getGUIEnvironment();

    fonts_.push_back(env->getFont("./misc/diablo12.xml"));
    fonts_.push_back(env->getFont("./misc/diablo14.xml"));
    fonts_.push_back(env->getFont("./misc/diablo16.xml"));
    fonts_.push_back(env->getFont("./misc/diablo18.xml"));
    fonts_.push_back(env->getFont("./misc/diablo28.xml"));
    fonts_.push_back(env->getFont("./misc/diablo36.xml"));
    fonts_.push_back(env->getFont("./misc/diablo48.xml"));

    addMusic("./music/01-intro.mp3");
    addMusic("./music/02-town.mp3");
    addMusic("./music/03-dungeon.mp3");
    addSoundEffect("./sounds/itemDrop.wav");
    addSoundEffect("./sounds/selectItem.wav");
    addSoundEffect("./sounds/goldDrop.wav");
    cout << "Loaded game Music" << endl;

    createMainScreen();
    cout << "MainScreen created." << endl;

    level_ = new Level(getSceneManager());
    cout << "Level created." << endl;

    controller_ = new XBOX360Controller();
    cout << "Controller created." << endl;

    mainCharacter_ = new MainCharacter(level_, getSceneManager(), getSoundEngine());
    cout << "Char created." << endl;

    grid_ = Grid(level_);
    cout << "Grid Created." << endl;

    sceneManager_->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    lights_.push_back(getSceneManager()->addLightSceneNode());
    cameras_.push_back(getSceneManager()->addCameraSceneNode(level_, DEFAULT_CAMERA_POSITION));
    
    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(),
            mainCharacter_,
            vector3df(5, 5, 5),
            core::vector3df(0, -2.0f, 0),
            core::vector3df(0, 0, 0), 0);

    mainCharacter_->addAnimator(anim);

    anim->drop();
}

Game::~Game() {
    //delete level_;
    delete controller_;
    delete mainCharacter_;
}