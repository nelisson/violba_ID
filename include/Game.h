#ifndef GAME_H
#define GAME_H

#include "MainCharacter.h"
#include "Monster.h"
#include "Level.h"
#include "Grid.h"
#include "XBOX360Controller.h"
#include "ItemGenerator.h"
#include <iostream>
#include <ctime>
#include "SoundEmmitter.h"

#define RESOLUTION_SCREEN core::dimension2d<u32>(1024, 683)
#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 35
#define DEFAULT_CAMERA_Z -40
#define DEFAULT_CAMERA_POSITION vector3df(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z)
#define DEFAULT_MONSTER_CREATION_TIME_IN_SECONDS 1
#define MAX_MONSTERS 1

#define GRAVITY 10
#define DEFAULT_MONSTER_GENERATION_CHANCE 100
#define DEFAULT_ITEM_GENERATION_CHANCE 100

using namespace std;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::gui;

namespace GameMusic {
    enum Music {
        INTRO,
        TOWN,
        DUNGEON,
    };
}

namespace GameFonts {
    enum Font {
        DIABLO12,
        DIABLO14,
        DIABLO16,
        DIABLO18,
        DIABLO28,
        DIABLO36,
        DIABLO48,
    };
}

enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_PLAY_DEMO_BUTTON,
};

class Game : public SoundEmmitter, 
             public IEventReceiver {

    private:
        bool needsRestart_;
        bool mainScreen_;
        bool isRunning_;
        bool isStatusVisible_;
        ISceneManager * sceneManager_;
        ItemGenerator itemGenerator_;
        Grid grid_;
        MicroPather pather_;
        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;
        vector<Monster*> monsters_;
        vector<IGUIFont*> fonts_;

        vector<ICameraSceneNode*> cameras_;
        vector<ILightSceneNode*> lights_;

        f32 elapsedTime_;
        time_t lastSpawn_;

        void createMainScreen();
        void createStatusSreen();

    protected:

    public:

        

        static void showStatus(void *userData);
        
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

        void clearCorpses();

        void printPath(vector<void*> path) const;

        void setCallbacks();
        bool doActions();
        vector<Monster*>::iterator attackMonster(vector<Monster*>::iterator monster);
        void attackMainCharacter(float damage);
        int attackMonsters();
        void tryGeneratingMonster(int chancePercent);
        void runMonstersAI();

        f32 getElapsedTime() { return elapsedTime_; }
        void setElapsedTime(f32 elapsedTime) { elapsedTime_ = elapsedTime; }

        static void moveCharacter(void*, core::vector2df desl);

        virtual bool OnEvent(const SEvent& event);

        void load();

        Game(ISceneManager * sceneManager, ISoundEngine * soundEngine);
        virtual ~Game();
};

#endif // GAME_H
