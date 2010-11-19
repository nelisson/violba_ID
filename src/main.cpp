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

#define RESOLUTION core::dimension2d<u32>(800, 600)
#define PROGRAM_NAME "Violba_ID"

using namespace irr;
using namespace scene;
using namespace std;


int main() {

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
			RESOLUTION, 32, false, false, false);

    Game game(device->getSceneManager());
    device->setEventReceiver(game.getController());

	core::array<SJoystickInfo> joystickInfo;
	device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
	device->setWindowCaption(programName.c_str());

	u32 then = device->getTimer()->getTime();

	while(device->run()) {
        game.doActions();

		u32 now = device->getTimer()->getTime();
		game.setElapsedTime((f32)(now - then) / 1000.f);
		then = now;

        device->getVideoDriver()->beginScene(true, true, 0);
		game.getSceneManager()->drawAll();
		device->getVideoDriver()->endScene();
	}

	device->drop();
	return 0;
}
