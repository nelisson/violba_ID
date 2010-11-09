#ifndef GAME_H
#define GAME_H

#include "MainCharacter.h"
#include "Monster.h"
#include "Level.h"
#include "XBOX360Controller.h"
#include <iostream>

using namespace std;
using namespace irr::scene;

class Game {
    private:
        ISceneManager * sceneManager_;

        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;


        vector<ICameraSceneNode*> cameras_;
        vector<ILightSceneNode*> lights_;

        void add(Node * node);

    protected:

    public:
        vector<Monster*> monsters_;

        ISceneManager * getSceneManager();

        Level * getLevel();
        XBOX360Controller * getController();
        MainCharacter * getMainCharacter();
        vector<Monster*>& getMonsters();

        vector<ICameraSceneNode*> getCameras();
        vector<ILightSceneNode*> getLights();

        void addMonster(Monster * monster);
        void addLight();
        void addCamera();

        Game(ISceneManager * sceneManager);
        virtual ~Game();
};

#endif // GAME_H
