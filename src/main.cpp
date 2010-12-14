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
#define CURSOR_IMAGE_PATH "./misc/cursor.bmp"

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

    device->setEventReceiver(&game);
    
    core::array<SJoystickInfo> joystickInfo;
    device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
    device->setWindowCaption(programName.c_str());

    u32 then = device->getTimer()->getTime();

    device->setWindowCaption(L"violba_ID");
    device->setResizable();
    
    IVideoDriver* driver = device->getVideoDriver();

    video::ITexture* cursor = driver->getTexture(CURSOR_IMAGE_PATH);
    driver->makeColorKeyTexture(cursor, SColor(255, 255, 255, 255));

    device->getCursorControl()->setVisible(false);

    while (device->run()) {
        u32 now = device->getTimer()->getTime();
        game.setElapsedTime((f32) (now - then) / 1000.f);
        then = now;

        driver->beginScene();
        if(!game.doActions())
            device->closeDevice();

        recti imageRect(0,0,cursor->getSize().Width, cursor->getSize().Height);
        driver->draw2DImage(cursor, device->getCursorControl()->getPosition(),
                            imageRect, 0, SColor(255, 255, 255, 255), true);

        driver->endScene();
    }

    device->drop();
    return 0;
}


