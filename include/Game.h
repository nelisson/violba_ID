#ifndef GAME_H
#define GAME_H

#include "MainCharacter.h"
#include "Monster.h"
#include "Level.h"
#include "XBOX360Controller.h"

using namespace std;
using namespace irr::scene;

class Game {
    private:
        ISceneManager * sceneManager_;

        Level * level_;
        XBOX360Controller * controller_;
        MainCharacter * mainCharacter_;
        vector<Monster*> monsters_;


    protected:

    public:
        ISceneManager * getSceneManager();

        Game();
        virtual ~Game();
};

#endif // GAME_H
