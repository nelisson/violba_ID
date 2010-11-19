#include "MainCharacter.h"

bool MainCharacter::walk(vector3df desl) {

    if(getState() == STOPPING || getState() == MOVING || getState() == JUMPING) {
        float moveHorizontal = desl.X;
        float moveVertical = desl.Z;
        double ang = sinal(moveHorizontal) * (180.0/PI) * acos(vector3df(0,0,1).dotProduct(vector3df(moveHorizontal,0,moveVertical).normalize()));
        if (ang >= 180)
            ang = 179;

        setRotation(vector3df(0, ang, 0));
        moveDelta(desl);
        if (getState() == STOPPING) {
            setFrameLoop(WALK);
            setState(MOVING);
        }

        return true;
    }
    return false;
}

void MainCharacter::slash(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(SLASH);
        thisptr->setAnimationSpeed(thisptr->getEquippedWeapon()->getAttackSpeed());
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::kick(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(KICK);
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::spin(void *userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(SPIN);
        thisptr->setAnimationSpeed(thisptr->getEquippedWeapon()->getAttackSpeed());
        thisptr->setState(ATTACK_STARTING);
    }
}

void MainCharacter::stop(void *userData, vector2df direction) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    thisptr->setFrameLoop(IDLE);
    thisptr->setState(STOPPING);
}

void MainCharacter::jump(void * userData) {
    MainCharacter * thisptr = (MainCharacter*) userData;
    if(thisptr->getState() != JUMPING && thisptr->getState() != ATTACK_STARTING && thisptr->getState() != ATTACK_ENDING) {
        thisptr->setFrameLoop(JUMP);
        thisptr->setState(JUMPING);
    }
}

void MainCharacter::levelUp() {
    if (getLevel() < DEFAULT_CHARACTER_MAX_LEVEL) {
        addLevels(1);
        experienceToLevelUp_ = experienceCurve(getLevel());

        cout << "Level Up! (current level " << getLevel() << ")" << endl;

        updateAttributes();
    }
}

void MainCharacter::updateAttributes() {
    strength_++;
    vitality_++;
    agility_++;

    increaseMaxHP(5);
    increaseMoveSpeed(3);

    speed_ += 3;
    jumpHeight_ += 5;
    fillHP();
}

void MainCharacter::setState(State state) {
    state_ = state;
}

float MainCharacter::getDamage() {
    return getEquippedWeapon()->getDamage() * strength_;
}

void MainCharacter::earnExperience(int experience) {
    if (getLevel() < DEFAULT_CHARACTER_MAX_LEVEL) {
        currentExperience_ += experience;

        if (currentExperience_ >= experienceToLevelUp_) {
            int overflowExperience = currentExperience_ - experienceToLevelUp_;

            currentExperience_ = experienceToLevelUp_;
            levelUp();
            earnExperience(overflowExperience);
        }
    }
}

long MainCharacter::experienceCurve(int level) {
    return 980 + 20*level*level;
}

void MainCharacter::OnAnimationEnd(IAnimatedMeshSceneNode *node) {
    node->setFrameLoop(IDLE);
    state_ = STOPPING;
}

void MainCharacter::refresh() {
    static f32 lastTimeBetweenFrames = 0;
    f32 middleFrame = (getEndFrame() + getStartFrame())/2.0;

    float jumpDelta = getJumpHeight() / (float)(getEndFrame() - getStartFrame());
    float timeBetweenFrames = getFrameNr() - (int)getFrameNr() - lastTimeBetweenFrames;
    lastTimeBetweenFrames = timeBetweenFrames;

    if(getState() == JUMPING) {
        if(getFrameNr() < middleFrame)
            moveDelta(vector3df(0, jumpDelta * timeBetweenFrames, 0));
    }
    else
        lastTimeBetweenFrames = 0;

    vector3df center = getAnimatedNode()->getBoundingBox().getCenter();

    cout << "Position X: " << getPosition().X << " Z: " << getPosition().Z << endl;
    cout << "Center X: " << center.X << " Z: " << center.Z << endl;
    cout << "Size X: " << (center.X)*2 << " Z:" << (center.Z)*2 << endl;
}

bool MainCharacter::tryHitCheck() {
    f32 middleFrame = (getEndFrame() + getStartFrame())/2.0;

    if (getState() == ATTACK_STARTING) {
        if ( (int)getFrameNr() == (int)middleFrame) {
            setState(ATTACK_ENDING);
            return true;
        }

        return false;
    }
    return false;
}

MainCharacter::MainCharacter(ISceneNode * parent,
                             ISceneManager * manager,
                             char * name,
                             char * modelPath,
                             int level,
                             int currentExperience,
                             int maxHP,
                             int vitality,
                             int strength,
                             int agility,
                             float moveSpeed,
                             float jumpHeight)
    : Character(parent, manager, name, modelPath, level, maxHP, moveSpeed),
      vitality_(vitality),
      strength_(strength),
      agility_(agility),
      jumpHeight_(jumpHeight){

    equippedWeapon_ = new Weapon("Espada");

    setAnimationEndCallback(this);
    getAnimatedNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    setFrameLoop(IDLE);
    setAnimationSpeed(20);
    setLoopMode(false);

    experienceToLevelUp_ += experienceCurve(level);
    earnExperience(currentExperience);

    state_ = STOPPING;
}

MainCharacter::~MainCharacter() {
    delete equippedWeapon_;
}
