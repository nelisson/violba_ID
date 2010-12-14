#include <irrlicht/irrlicht.h>
#include <math.h>
#include <irrlicht/driverChoice.h>
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
            
    

    Game game(device->getSceneManager());
    game.getController()->device_ = device;
    device->setEventReceiver(game.getController());

    core::array<SJoystickInfo> joystickInfo;
    device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
    device->setWindowCaption(programName.c_str());

    u32 then = device->getTimer()->getTime();

    cout << "Char size X: " << game.getMainCharacter()->getSize().Width << " Y:" << game.getMainCharacter()->getSize().Height << endl;

    device->setWindowCaption(L"violba_ID");
    device->setResizable();


    IGUIEnvironment* env = device->getGUIEnvironment();

    /*
    To make the font a little bit nicer, we load an external font
    and set it as the new default font in the skin.
    To keep the standard font for tool tip text, we set it to
    the built-in font.
     */

    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("./models/diablo28.xml");
    if (font)
        skin->setFont(font);

    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

    /*
    And at last, we create a nice Irrlicht Engine logo in the top left corner.
     */
    env->addImage(device->getVideoDriver()->getTexture("./models/mainScreen.png"),
            position2d<int>(0, 0));

    /*
    We add three buttons. The first one closes the engine. The second
    creates a window and the third opens a file open dialog. The third
    parameter is the id of the button, with which we can easily identify
    the button in the event receiver.
     */

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

        device->getVideoDriver()->beginScene(true, true, 0);
        if(game.mainScreen){
            env->drawAll();
            game.playMusic(TOWN);
        }
        else{
            game.playMusic(DUNGEON);
            game.doActions();
            game.getSceneManager()->drawAll();
        }
        device->getVideoDriver()->endScene();
    }

    device->drop();
    return 0;
}


