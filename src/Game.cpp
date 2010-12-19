#include "Game.h"

using namespace GameMusic;
using namespace GameFonts;
using namespace State;

void Game::addMonster(Monster * monster) {
    monsters_.push_back(monster);

    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(level_->getTriangleSelector(),
            monster, core::vector3df(5, 5, 5),
            core::vector3df(0, GRAVITY, 0),
            core::vector3df(0, -5, 0), 0);
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
    controller_->setCallBack(BACK, PRESSED, mainCharacter_->drinkPotion, mainCharacter_);
    controller_->setCallBack(L_ANALOG, PRESSED, this->moveCharacter, this);
    controller_->setCallBack(L_ANALOG, RELEASED, mainCharacter_->stop, mainCharacter_);
    controller_->setCallBack(L, PRESSED, mainCharacter_->crouch, mainCharacter_);
    controller_->setCallBack(L, RELEASED, mainCharacter_->getUp, mainCharacter_);
    controller_->setCallBack(R, PRESSED, mainCharacter_->block, mainCharacter_);
    controller_->setCallBack(R, HOLD, mainCharacter_->block, mainCharacter_);
    controller_->setCallBack(R, RELEASED, mainCharacter_->unblock, mainCharacter_);

    controller_->setCallBack(START, PRESSED, this->showStatus, this);    

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

    vector3df temp;
    ISceneNode* intersectedNode = cursor_->getIntersectedSceneNode(getSceneManager(), temp);
    if (intersectedNode) {
        if ( (intersectedNode->getID() & NodeIDFlags::ENEMY) == NodeIDFlags::ENEMY ) {
            cursor_->setFilter(CursorColors::ATTACKING);
        }
        else if ( (intersectedNode->getID() & NodeIDFlags::ITEM) == NodeIDFlags::ITEM ) {
            cursor_->setFilter(CursorColors::GETTING);
        }
        else
            cursor_->setFilter(CursorColors::POINTING);
    }
    else
        cursor_->setFilter(CursorColors::POINTING);
    
    if (isStatusVisible_) {
        sceneManager_->getVideoDriver()->draw2DRectangle(SColor(255, 120, 120, 120), rect<s32> (0, 0, 1024, 683));
        mainCharacter_->getInventory()->drawInventory();
        sceneManager_->getGUIEnvironment()->drawAll();
        cursor_->render();
        return true;
    }
    if (mainScreen_) {
        sceneManager_->getGUIEnvironment()->drawAll();
        cursor_->render();
        return isRunning_;
    }

    if (needsRestart_) {
        sceneManager_->getGUIEnvironment()->clear();
        reset();
        needsRestart_ = false;
    }

    
    clearCorpses();
    getSceneManager()->drawAll();
    cursor_->render();

    IGUIStaticText *kills = sceneManager_->getGUIEnvironment()->addStaticText(toWchar_Kills(killCounter_),
            rect<s32>(800,0,1000,50),true,true, 0,-1,true);
    kills->setTextAlignment(EGUIA_CENTER ,EGUIA_CENTER );
    kills->draw();



    cameras_[0]->setTarget(vector3df(mainCharacter_->getPosition().X,
                                     getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                                     mainCharacter_->getPosition().Z));

    cameras_[0]->setPosition(vector3df(mainCharacter_->getPosition().X,
                                       getLevel()->getTerrain()->getHeight(mainCharacter_->getPosition().X, mainCharacter_->getPosition().Z),
                                       mainCharacter_->getPosition().Z) + DEFAULT_CAMERA_POSITION);



    if (mainCharacter_->isAlive()) {
        mainCharacter_->refresh(elapsedTime_);

        if (mainCharacter_->tryHitCheck()) {
            cout << "Hits: " << attackMonsters();
        }

        if (mainCharacter_->getState() == GETTING_ITEM) {

            try {
                vector<Cell*> items = grid_.getItemCells(mainCharacter_->getGridRectangle());

                int counter = 0;

                vector<Cell*>::iterator i;
                for (i = items.begin(); i < items.end(); i++) {
                    
                    try {
                        mainCharacter_->getInventory()->putItem((*i)->getItem());
                        (*i)->getItem()->setVisible(false);
                        (*i)->removeItem();
                        counter++;
                    }
                    catch(int j) {
                        cout << "Inventory is full." <<endl;
                        playSoundEffect(Sounds::INV_FULL);

                    }
                }

                cout << "Got " << counter << " items." << endl;
            }            catch (int i) {
            }

            catch(int) {}
            

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
        if ((*i)->getState() == DEAD) {
            (*i)->decreaseCorspeDelay(elapsedTime_);

            if ((*i)->getCorspeDelay() < 0) {
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
        (*monster)->getAnimatedNode()->setID(NodeIDFlags::CORPSE);
        killCounter_++;

        try {

            cout << "Vo dropa." << endl;
            Item * droppedItem = itemGenerator_.dropItem(DEFAULT_ITEM_GENERATION_CHANCE);
            playSoundEffect(Sounds::GOLD_DROP);

            cout << "DropedItem OK." << endl;
            Item * item;
            
            item = droppedItem->copy(level_, getSceneManager());
            
            if(item->getNamex().compare("Potion")==0){
                item->setScale(vector3df(0.025,0.025,0.025));
            }

            if(item->getNamex().compare("Armor")==0){
                item->setScale(vector3df(0.1,0.1,0.1));
            }

            cout << "ItemCopy OK." << endl;

            vector3df position = vector3df((*monster)->getGridPosition().X,
                    0,
                    (*monster)->getGridPosition().Y);

            position.Y = getLevel()->getTerrain()->getHeight(position.X, position.Z);

            if(item->getNamex().compare("Armor")==0){
                item->setScale(vector3df(0.11,0.11,0.11));
                position += vector3df(0,2,0);
            }

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

        if ((*monster)->isAlive()) {

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
        Monster * newMonster;
        if(randomBetween(0,1)){
            newMonster = new Monster(level_, sceneManager_, getSoundEngine(), DEFAULT_MONSTER_MESH1);
        }
        else{
            newMonster = new Monster(level_, sceneManager_, getSoundEngine(),DEFAULT_MONSTER_MESH2);
        }

        addMonster(newMonster);

        dimension2df size = getLevel()->getSize();

        float randomX = randomBetween(0, size.Width);
        float randomZ = randomBetween(0, size.Height);
        float Y = getLevel()->getTerrain()->getHeight(randomX, randomZ);

        newMonster->setPosition(vector3df(randomX, Y, randomZ));
    }
}

void Game::printPath(vector<void*> path) const {
    cout << "PATH size:" << path.size() << endl;

    vector<void*>::const_iterator i;
    for (i = path.begin(); i < path.end(); i++) {
        cout << "X: " << ((Cell*)(*i))->getPosition().X;
        cout << " Y: " << ((Cell*)(*i))->getPosition().Y << endl;
    }
}

void Game::runMonstersAI() {
    vector<Monster*>::iterator monster;
    for (monster = monsters_.begin(); monster < monsters_.end(); monster++) {
        if ((*monster)->isAlive()) {
            vector3df ninjaPosition = mainCharacter_->getAbsolutePosition();
            vector3df monsterPosition = (*monster)->getPosition();

            if (ninjaPosition.getDistanceFrom(monsterPosition) > (*monster)->getRange()) {
                vector3df vetor = ninjaPosition - monsterPosition;
                vetor.normalize();
                (*monster)->walk(vetor * getElapsedTime());
                if ((*monster)->getState() == STOPPING) {
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

    cout << "VO playa" << endl;
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
    } else if (event.EventType == EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.isLeftPressed()) {
            
            vector3df collisionPoint;
            ISceneNode* selectedNode = cursor_->getIntersectedSceneNode(getSceneManager(), collisionPoint);
            mainCharacter_->setTarget(selectedNode);
            mainCharacter_->setGoto(collisionPoint);

            cout << "Collision Point X: "<< collisionPoint.X ;
            cout << " Y: "<< collisionPoint.Y ;
            cout << " Z: "<< collisionPoint.Z << endl;

            if ((selectedNode->getID() & NodeIDFlags::FLOOR) == NodeIDFlags::FLOOR) {
                vector3df difference = collisionPoint - mainCharacter_->getPosition();
                moveCharacter(this, vector2df(difference.X, difference.Z));
            }
        }

    } else
        return false;
}

void Game::createStatusSreen() {
    IGUIEnvironment* env = sceneManager_->getGUIEnvironment();
    IGUIStaticText* text;
    const char* Temp;
    char* num = (char*) malloc(30);
    rect<s32> tamanho;
    position2di position;
    dimension2d<u32> screen = RESOLUTION_SCREEN;

    Temp = "Status";
    tamanho = getStringSize(Temp, 48);
    position = position2di((screen.Width - tamanho.getWidth()) / 2, 0);
    text = env->addStaticText(L"Status", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO48));
    text->setRelativePosition(position);

    Temp = "HP:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, screen.Height / 6 - tamanho.getHeight() / 2);
    text = env->addStaticText(L"HP:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%.0f/", mainCharacter_->getCurrentHP());
    position += position2di(tamanho.getWidth(), 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getCurrentHP(), true), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%.0f", mainCharacter_->getMaxHP());
    position += position2di(tamanho.getWidth()*(float) 3 / 4, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getMaxHP()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Level:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*1.25 - tamanho.getHeight() / 2);
    text = env->addStaticText(L"Level:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%d", mainCharacter_->getLevel());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getLevel()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Vitality:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*1.5 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"Vitality:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%d", mainCharacter_->getVitality());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getVitality()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Agility:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*1.75 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"Agility:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%d", mainCharacter_->getAgility());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getAgility()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Strength:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*2 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"Strength:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%d", mainCharacter_->getStrength());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getStrength()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Exp:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*2.25 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"Exp:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%ld/", mainCharacter_->getCurrentExperience());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getCurrentExperience(), true), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%ld", mainCharacter_->getExperienceToLevelUp());
    position += position2di(tamanho.getWidth(), 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getExperienceToLevelUp()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "jumpHeight:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*2.50 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"JumpHeight:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%.0f", mainCharacter_->getJumpHeight());
    position += position2di(tamanho.getWidth() / 1.5, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getJumpHeight()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "MoveSpeed:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*2.75 - tamanho.getHeight() / 3);
    position += position2di(0, 30);
    text = env->addStaticText(L"MoveSpeed:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    sprintf(num, "%.0f", mainCharacter_->getMoveSpeed());
    position += position2di(tamanho.getWidth() / 1.4, 0);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getMoveSpeed()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Position:";
    tamanho = getStringSize(Temp, 28);
    position = position2di(30, (screen.Height / 6)*3.1 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    text = env->addStaticText(L"Position:", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    vector3df mainPosition = mainCharacter_->getPosition();

    sprintf(num, "X=%.0f Y=%.0f Z=%.0f", mainPosition.X, mainPosition.Y, mainPosition.Z);
    position = position2di(30, (screen.Height / 6)*3.35 - tamanho.getHeight() / 2);
    position += position2di(0, 30);
    tamanho = getStringSize(num, 28);
    text = env->addStaticText(toWchar_T(mainCharacter_->getPosition()), tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);

    Temp = "Inventory";
    tamanho = getStringSize(Temp, 28);
    position = position2di(650, 100);
    text = env->addStaticText(L"Inventory", tamanho);
    text->setOverrideFont(fonts_.at(DIABLO28));
    text->setRelativePosition(position);
    
    //exp -> current nextLevel
    //jumpHeight
    //(500, 150,1000, 659)
    //moveSpeed
    //AttackSpeed
    //Position
}

void Game::showStatus(void *userData) {
    Game * thisptr = (Game*) userData;

    if (!thisptr->mainScreen_) {

        if (!thisptr->isStatusVisible_) {
            thisptr->isStatusVisible_ = true;
            thisptr->getSceneManager()->getGUIEnvironment()->clear();
            thisptr->createStatusSreen();
            thisptr->getSceneManager()->getGUIEnvironment()->drawAll();
        }
        else{
            thisptr->isStatusVisible_ = false;
            thisptr->getSceneManager()->getGUIEnvironment()->clear();
        }
    }
}
void Game::reset() {
    dimension2df terrainSize = getLevel()->getSize();

    float levelHeight = getLevel()->getTerrain()->getHeight(terrainSize.Width / 2,
                                                            terrainSize.Height / 2);
    vector3df levelCenter(terrainSize.Width / 2, levelHeight, terrainSize.Height / 2);


    killCounter_ = 0;
    mainCharacter_->reset(levelCenter);

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

void Game::startGame(void *userData) {
    ((Game*)userData)->playSoundEffect(Sounds::SELECTION);
    ((Game*)userData)->mainScreen_ = false;
    ((Game*)userData)->needsRestart_ = true;
}

Game::Game(IrrlichtDevice* device, ISceneManager * sceneManager, ISoundEngine * soundEngine)

    : SoundEmmitter(soundEngine),
      isStatusVisible_(false),
      isRunning_(true),
      mainScreen_(true),
      sceneManager_(sceneManager),
      pather_(&grid_) {

    killCounter_=0;
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
    addSoundEffect("./sounds/inventoryFull.wav");
    cout << "Loaded game Music" << endl;

    createMainScreen();
    cout << "MainScreen created." << endl;

    level_ = new Level(getSceneManager());
    cout << "Level created." << endl;

    controller_ = new XBOX360Controller();
    cout << "Controller created." << endl;

    controller_->setCallBack(X, PRESSED, startGame, this);

    mainCharacter_ = new MainCharacter(level_, getSceneManager(), getSoundEngine());
    cout << "Char created." << endl;

    grid_ = Grid(level_);
    cout << "Grid Created." << endl;

    cursor_ = new Cursor(device, getSceneManager()->getVideoDriver());

    sceneManager_->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    lights_.push_back(getSceneManager()->addLightSceneNode());
    cameras_.push_back(getSceneManager()->addCameraSceneNode(level_, DEFAULT_CAMERA_POSITION));

    
    ISceneNodeAnimator* anim = sceneManager_->createCollisionResponseAnimator(
            level_->getTriangleSelector(),

            mainCharacter_,
            vector3df(5, 5, 5),
            core::vector3df(0, GRAVITY, 0),
            core::vector3df(0, -5, 0), 0);

    mainCharacter_->addAnimator(anim);

    anim->drop();
}

Game::~Game() {
    //delete level_;
    delete controller_;
    delete mainCharacter_;
}