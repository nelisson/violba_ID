#ifndef GAME_H
#define GAME_H

#include "MainCharacter.h"
#include "Monster.h"
#include "Level.h"
#include "Grid.h"
#include "XBOX360Controller.h"
#include <iostream>

#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 35
#define DEFAULT_CAMERA_Z -40
#define DEFAULT_CAMERA_POSITION vector3df(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z)

#define DEFAULT_MONSTER_GENERATION_CHANCE_PER_FRAME 1

using namespace std;
using namespace irr::scene;
using namespace irr::core;

class Game {
    private:
        ISceneManager * sceneManager_;
        Grid grid_;

        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;
        vector<Monster*> monsters_;

        vector<ICameraSceneNode*> cameras_;
        vector<ILightSceneNode*> lights_;

        f32 elapsedTime_;

    protected:

    public:
        ISceneManager * getSceneManager() { return sceneManager_; }

        Level * getLevel() { return level_; }
        XBOX360Controller * getController() { return controller_; }
        MainCharacter * getMainCharacter() { return mainCharacter_; }
        vector<Monster*> getMonsters() { return monsters_; }
        Monster* getMonster(int i) { return monsters_.at(i); }

        vector<ICameraSceneNode*> getCameras() { return cameras_; }
        vector<ILightSceneNode*> getLights() { return lights_; }

        void addMonster(Monster * monster);
        vector<Monster*>::iterator removeMonster(vector<Monster*>::iterator monster);


        void setCallbacks();
        void doActions();
        vector<Monster*>::iterator attackMonster(vector<Monster*>::iterator monster);
        void attackMainCharacter(float damage);
        int attackMonsters();
        void tryGeneratingMonster(int chancePercent);
        void runMonstersAI();

        f32 getElapsedTime() { return elapsedTime_; }
        void setElapsedTime(f32 elapsedTime) { elapsedTime_ = elapsedTime; }

        static void moveCharacter(void*, core::vector2df desl);


        Game(ISceneManager * sceneManager);
        virtual ~Game();
};

#endif // GAME_H
