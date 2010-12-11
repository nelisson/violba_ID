#include <irrlicht/irrlicht.h>
#include <math.h>
#include "XBOX360Controller.h"
#include "Utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Game.h"

#include "MainCharacter.h"
#include "Monster.h"

#define RESOLUTION core::dimension2d<u32>(1024, 683)
#define PROGRAM_NAME "Violba_ID"

using namespace irr;
using namespace scene;
using namespace std;
using namespace core;
using namespace video;
using namespace io;
using namespace gui;

int main() {

    IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
            RESOLUTION, 32, false, false, false);

    ISoundEngine* sound = createIrrKlangDevice();

    Game game(device->getSceneManager(), sound);

    cout << "Created game" << endl;

    game.getController()->device_ = device;
    device->setEventReceiver(game.getController());

    core::array<SJoystickInfo> joystickInfo;
    device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
    device->setWindowCaption(programName.c_str());

    u32 then = device->getTimer()->getTime();

    device->setWindowCaption(L"violba_ID");
    device->setResizable();

    IGUIEnvironment* env = device->getGUIEnvironment();

    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("./models/diablo28.xml");
    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    env->addImage(device->getVideoDriver()->getTexture("./models/mainScreen.png"),
            position2d<int>(0, 0));

    int deslocX = 200, deslocY = 50;
    int x0 = 70, y0 = 520, y1 = y0 + 60;

    env->addButton(rect<s32 > (x0, y0, x0 + deslocX, y0 + deslocY), 0, GUI_ID_PLAY_DEMO_BUTTON,
            L"Play Demo");
    env->addButton(rect<s32 > (x0, y1, x0 + deslocX, y1 + deslocY), 0, GUI_ID_QUIT_BUTTON,
            L"Quit");   

    while (device->run()) {
        u32 now = device->getTimer()->getTime();
        game.setElapsedTime((f32) (now - then) / 1000.f);
        then = now;

        device->getVideoDriver()->beginScene();
        if(game.mainScreen){
            env->drawAll();
            game.playMusic(GameMusic::TOWN, true, true, 10000);
        }
        else{
            game.playMusic(GameMusic::DUNGEON, true, true, 1000);
            game.doActions();
            game.getSceneManager()->drawAll();
        }
        device->getVideoDriver()->endScene();
    }

    device->drop();
    return 0;
}


