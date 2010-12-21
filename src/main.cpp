#include <irrlicht/irrlicht.h>
#include <iostream>

#include "Game.h"

#define RESOLUTION core::dimension2du(1024, 683)
#define PROGRAM_NAME "Violba ID"

using namespace irr;
using namespace std;
using namespace core;

int main( int argc, char* argv[]) {

    IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
            RESOLUTION, 32, false, true, false);

    stringw programName = PROGRAM_NAME;
    array<SJoystickInfo> joystickInfo;

    device->activateJoysticks(joystickInfo);
    device->setWindowCaption(programName.c_str());
    device->setResizable();
    device->getCursorControl()->setVisible(false);

    Game game(device, device->getSceneManager(), createIrrKlangDevice(), argc > 1 ? ControllerType::PLAYSTATION2 :  ControllerType::XBOX360);
    cout << "Created game" << endl;

    device->setEventReceiver(&game);
   
    u32 then = device->getTimer()->getTime();
    
    while (device->run()) {
        u32 now = device->getTimer()->getTime();
        game.setElapsedTime((f32) (now - then) / 1000.f);
        then = now;

        device->getVideoDriver()->beginScene();
        if(!game.doActions()) {
            device->closeDevice();
            sleep(1);
        }

        device->getVideoDriver()->endScene();
    }

    device->drop();
    return 0;
}