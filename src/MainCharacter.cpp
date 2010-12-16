#include "MainCharacter.h"

using namespace State;

void MainCharacter::drinkPotion(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;

    thisptr->heal(DEFAULT_POTION_HEAL);
    thisptr->playSoundEffect(Sounds::POTION);
}

bool MainCharacter::walk(vector3df desl) {

    float moveHorizontal = desl.X;
    float moveVertical = desl.Z;
    double ang = sinal(moveHorizontal) * (180.0 / PI) * acos(vector3df(0, 0, 1).dotProduct(vector3df(moveHorizontal, 0, moveVertical).normalize()));
    if (ang >= 180)
        ang = 179;

    setRotation(vector3df(0, ang, 0));

    if (getState() == STOPPING || 
        getState() == MOVING ||
        getState() == JUMPING ||
        getState() == DOUBLE_JUMPING) {
        
        moveDelta(desl);

        if (getState() == STOPPING) {
            setFrameLoop(ANIM_WALK);
            setState(MOVING);
        }

        return true;
    }
    return false;
}

void MainCharacter::slash(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD &&
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING) {
        
        thisptr->setFrameLoop(ANIM_SLASH);

        if (randomBetween(0,100) > 50)
            thisptr->playSoundEffect(Sounds::SWING1);
        else
            thisptr->playSoundEffect(Sounds::SWING2);

        thisptr->setAnimationSpeed(thisptr->getEquippedWeapon()->getAttackSpeed());
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::kick(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD && 
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING) {
        
        thisptr->setFrameLoop(ANIM_KICK);
        thisptr->playSoundEffect(Sounds::KICK);
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::spin(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD && 
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING) {
        
        thisptr->setFrameLoop(ANIM_SPIN);
        thisptr->playSoundEffect(Sounds::SPIN);
        thisptr->setAnimationSpeed(thisptr->getEquippedWeapon()->getAttackSpeed());
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::stop(void *userData, vector2df direction) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    thisptr->setFrameLoop(ANIM_IDLE);
    thisptr->setState(STOPPING);
}

void MainCharacter::jump(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() == JUMPING) {
        doubleJump(thisptr);
    }

    if (thisptr->getState() != DEAD &&
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING) {

        thisptr->setFrameLoop(ANIM_JUMP);
        thisptr->playSoundEffect(Sounds::JUMP);
        thisptr->setState(JUMPING);
    }
}

void MainCharacter::doubleJump(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() == JUMPING) {
        thisptr->setFrameLoop(ANIM_FRONTFLIP);
        thisptr->playSoundEffect(Sounds::JUMP);
        thisptr->setState(DOUBLE_JUMPING);
    }
}

void MainCharacter::crouch(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD &&
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING &&
        thisptr->getState() != CROUCHING) {

        thisptr->setFrameLoop(ANIM_CROUCH);
        thisptr->setState(CROUCHING);
    }
}

void MainCharacter::getUp(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD &&
        thisptr->getState() != DYING &&
        thisptr->getState() != JUMPING &&
        thisptr->getState() != DOUBLE_JUMPING &&
        thisptr->getState() != ATTACK_STARTING &&
        thisptr->getState() != ATTACK_ENDING) {

        thisptr->setFrameLoop(ANIM_GET_UP);
        thisptr->setState(STOPPING);
    }
}

void MainCharacter::block(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != BLOCKING && thisptr->getState() != DEAD && thisptr->getState() != DYING && thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(ANIM_BLOCK);
        thisptr->setState(BLOCKING);
    }
}

void MainCharacter::unblock(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if (thisptr->getState() != DEAD && thisptr->getState() != DYING && thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(ANIM_UNBLOCK);
        thisptr->setState(STOPPING);
    }
}

void MainCharacter::levelUp() {
    if (getLevel() < DEFAULT_CHARACTER_MAX_LEVEL) {
        addLevels(1);
        playSoundEffect(Sounds::LEVEL_UP);

        cout << "Level Up! (current level " << getLevel() << ")" << endl;

        updateAttributes();
    }
}

void MainCharacter::die() {
    if (getState() != DYING && getState() != DEAD) {
        playSoundEffect(Sounds::DEAD);
        setFrameLoop(ANIM_DEATH_BACKWARDS);
        setState(DYING);
    }
}

void MainCharacter::updateAttributes() {
    strength_ = STARTING_STRENGTH + getLevel() * 1;
    vitality_ = STARTING_VITALITY + getLevel() * 1;
    agility_  = STARTING_AGILITY  + getLevel() * 1;

    setMaxHP(STARTING_HP + getLevel() * 5);
    //increaseMoveSpeed(3);
    speed_      = DEFAULT_CHARACTER_MOVESPEED;
    jumpHeight_ = STARTING_JUMP_HEIGHT;

    currentExperience_ = experienceCurve(getLevel() - 1);
    experienceToLevelUp_ = experienceCurve(getLevel());
    fillHP();
}

float MainCharacter::getDamage() const {
    return getEquippedWeapon()->getDamage() * strength_;
}

void MainCharacter::earnExperience(int experience) {
    if (getLevel() < DEFAULT_CHARACTER_MAX_LEVEL) {
        currentExperience_ += experience;

        cout<<"XP to level: " << experienceToLevelUp_;
        cout<<"XP gained: " << experience;

        if (currentExperience_ >= experienceToLevelUp_) {
            int overflowExperience = currentExperience_ - experienceToLevelUp_;

            currentExperience_ = experienceToLevelUp_;
            levelUp();
            earnExperience(overflowExperience);
        }
    }
}

void MainCharacter::OnAnimationEnd(IAnimatedMeshSceneNode *node) {
    switch (getState()) {
        case DYING :
        case DEAD  :
            cout << "GameOver" << endl;
            setState(DEAD);
            break;

        case BLOCKING :
            setFrameLoop(ANIM_BLOCKFREEZE);
            break;

        case CROUCHING :
            setFrameLoop(ANIM_CROUCHFREEZE);
            setState(GETTING_ITEM);
            break;

        default :
            node->setFrameLoop(ANIM_IDLE);
            setState(STOPPING);
    }
}

void MainCharacter::refresh() {
    static f32 lastFrameNumber = 102;
    f32 middleFrame = (getEndFrame() + getStartFrame()) / 2.0;

    float jumpDelta = getJumpHeight() / (float) (middleFrame - getStartFrame());
    f32 timeBetweenFrames = getFrameNr() - lastFrameNumber;

    irr::core::list<ISceneNodeAnimator*>::ConstIterator animIt = getAnimators().getLast();
    ISceneNodeAnimatorCollisionResponse* anim = (ISceneNodeAnimatorCollisionResponse*) * animIt;

    switch (getState()) {
        case JUMPING: {
            if (getFrameNr() < middleFrame) {
                anim->setGravity(vector3df(0, 0, 0));
                moveDelta(vector3df(0, jumpDelta * timeBetweenFrames, 0));
                lastFrameNumber = getFrameNr();
                anim->setGravity(vector3df(0, -2.0f, 0));
            }         
            else {
                //anim->setGravity(vector3df(0,-10.0f,0));
                //if(middleFrame - lastFrameNumber > 0)
                moveDelta(vector3df(0, jumpDelta * (middleFrame - lastFrameNumber), 0));
                lastFrameNumber = middleFrame;
            }
            //lastFrameNumber = getFrameNr();
            //        cout << "timeBetweenFrames: " << timeBetweenFrames << endl;
            break;
        }

        default: {
            lastFrameNumber = 102;
        }
    }

//    vector3df center = getAnimatedNode()->getBoundingBox().getCenter();

    //cout << "Position X: " << getPosition().X << " Z: " << getPosition().Z << endl;
    //cout << "Center X: " << center.X << " Z: " << center.Z << endl;
    //cout << "Size X: " << (center.X)*2 << " Z:" << (center.Z)*2 << endl;
}

bool MainCharacter::tryHitCheck() {
    f32 middleFrame = (getEndFrame() + getStartFrame()) / 2.0;

    if (getState() == ATTACK_STARTING) {
        if ((int) getFrameNr() == (int) middleFrame) {
            setState(ATTACK_ENDING);
           
            return true;
        }

        return false;
    }
    return false;
}

MainCharacter::MainCharacter(ISceneNode * parent,
        ISceneManager * manager,
        ISoundEngine * soundEngine,
        vector3df offset,
        const char * name,
        const char * modelPath,
        int level)
    : Character(parent, manager,
                soundEngine,
                offset, name,
                modelPath, level) {

    cout<<"CreatingSword"<<endl;
    equippedWeapon_ = new Weapon(NULL, NULL, "Espada");
    cout<<"CreatedSword"<<endl;
    inventory_ = new Inventory();
    printf("%p\n", inventory_);

    addSoundEffect("./sounds/heal.wav");
    addSoundEffect("./sounds/charHit.wav");
    addSoundEffect("./sounds/levelUp.wav");
    addSoundEffect("./sounds/swordSwing.wav");
    addSoundEffect("./sounds/charDead.wav");
    addSoundEffect("./sounds/axeSwing.wav");
    addSoundEffect("./sounds/timeToDie.wav");
    addSoundEffect("./sounds/walk2.wav");
    addSoundEffect("./sounds/kick.wav");
    addSoundEffect("./sounds/spinAttack.wav");
    addSoundEffect("./sounds/jump.wav");
    addSoundEffect("./sounds/block.wav");

    vector3df center = getAnimatedNode()->getBoundingBox().getCenter();
    cout << "CHAR KASLDSALD Height: " << center.Y * 2;

    setSize(dimension2df(5,5));

    setAnimationEndCallback(this);
    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setFrameLoop(ANIM_IDLE);
    setAnimationSpeed(20);
    setLoopMode(false);

    updateAttributes();

    setState(STOPPING);
}

MainCharacter::~MainCharacter() {
    delete equippedWeapon_;
}
