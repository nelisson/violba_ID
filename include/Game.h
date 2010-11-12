#ifndef GAME_H
#define GAME_H

#include "MainCharacter.h"
#include "Monster.h"
#include "Level.h"
#include "XBOX360Controller.h"
#include <iostream>

#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 35
#define DEFAULT_CAMERA_Z -40

using namespace std;
using namespace irr::scene;
using namespace irr::core;

class Game {
    private:
        ISceneManager * sceneManager_;

        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;
        vector<Monster*> * monsters_;

        vector<ICameraSceneNode*> cameras_;
        vector<ILightSceneNode*> lights_;

        f32 elapsedTime_;

        void add(Node * node);

    protected:

    public:
        ISceneManager * getSceneManager() { return sceneManager_; }

        Level * getLevel() { return level_; }
        XBOX360Controller * getController() { return controller_; }
        MainCharacter * getMainCharacter() { return mainCharacter_; }
        vector<Monster*> * getMonsters() { return monsters_; }

        vector<ICameraSceneNode*> getCameras() { return cameras_; }
        vector<ILightSceneNode*> getLights() { return lights_; }

        void addMonster(Monster * monster);

        void setCallbacks();
        void doActions();
        vector<Monster*>::iterator attackMonster(vector<Monster*>::iterator monster);
        int attackMonsters();

        f32 getElapsedTime() { return elapsedTime_; }
        void setElapsedTime(f32 elapsedTime) { elapsedTime_ = elapsedTime; }

        static void moveCharacter(void*, core::vector2df desl);

        Game(ISceneManager * sceneManager);
        virtual ~Game();
};

#endif // GAME_H
