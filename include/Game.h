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

#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 35
#define DEFAULT_CAMERA_Z -40
#define DEFAULT_CAMERA_POSITION vector3df(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z)
#define DEFAULT_MONSTER_CREATION_TIME_IN_SECONDS 4

#define DEFAULT_MONSTER_GENERATION_CHANCE 100
#define DEFAULT_ITEM_GENERATION_CHANCE 60

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

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_PLAY_DEMO_BUTTON,
};

class Game : public SoundEmmitter, public IEventReceiver {
    private:
        bool mainScreen_;
        bool isRunning_;
        bool isStatusVisible_;
        ISceneManager * sceneManager_;
        ItemGenerator itemGenerator_;
        Grid grid_;
        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;
        vector<Monster*> monsters_;

        vector<ICameraSceneNode*> cameras_;
        vector<ILightSceneNode*> lights_;

        void createMainScreen();
        f32 elapsedTime_;
        time_t lastSpawn_;

    protected:

    public:

        static void showStatus(void *userData);
        static void hideStatus(void *userData);
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

        vector<Weapon> loadWeapons();
        vector<Armor> loadArmors();
        vector<Item> createItems();

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
        
        Game(ISceneManager * sceneManager, ISoundEngine * soundEngine);
        virtual ~Game();
};

#endif // GAME_H
