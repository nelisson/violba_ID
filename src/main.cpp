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

    MainCharacter * ninja          = game.getMainCharacter();
    ICameraSceneNode * camera      = game.getCameras()[0];
    ILightSceneNode * light        = game.getLights()[0];
    XBOX360Controller * controller = game.getController();
    vector<Monster*> * monsters    = game.getMonsters();

    game.setCallbacks();

	core::array<SJoystickInfo> joystickInfo;
	device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
	device->setWindowCaption(programName.c_str());

	video::IVideoDriver* driver = device->getVideoDriver();

    ninja->getNode()->setAnimationEndCallback(ninja);
    ninja->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    ninja->getNode()->setFrameLoop(IDLE);
    ninja->getNode()->setAnimationSpeed(20);
    ninja->getNode()->setLoopMode(false);
    ninja->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);

	u32 then = device->getTimer()->getTime();

	while(device->run()) {
	    /*
	    cout << "Rotation X: " << ninja->getNode()->getRotation().X;
	    cout << " Y: " << ninja->getNode()->getRotation().Y;
	    cout << " Z: " << ninja->getNode()->getRotation().Z << endl;
	    core::vector3df ninjaDirection = ninja->getNode()->getRotation().rotationToDirection();
	    cout << "Direction X: " << ninjaDirection.X;
	    cout << " Y: " << ninjaDirection.Y;
	    cout << " Z: " << ninjaDirection.Z << endl;
        */

        game.doActions();
        // cubos

        int random = rand() % 1000;
        if (random < 10) {
            Monster * newMonster = new Monster("Cubo da morte", "./models/teste.md2", 100, 10, 1);

            game.addMonster(newMonster);

            int randomx = (rand() % 100) - 50;
            int randomz = (rand() % 100) - 50;

            newMonster->getNode()->OnAnimate(0);
            newMonster->getNode()->setPosition(core::vector3df(randomx, 0.0, randomz));
            newMonster->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
            newMonster->getNode()->setAnimationSpeed(30);
            newMonster->getNode()->setLoopMode(false);
            newMonster->getNode()->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
            newMonster->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        for (int i = 0; i < monsters->size(); i++) {
            if (ninja->getNode()->getPosition().getDistanceFrom(monsters->at(i)->getNode()->getPosition()) > monsters->at(i)->getRange()) {
                core::vector3df vetor = ninja->getNode()->getPosition() - monsters->at(i)->getNode()->getPosition();
                vetor.normalize();
                monsters->at(i)->getNode()->setPosition( monsters->at(i)->getNode()->getPosition() + (vetor * 0.2) );
            }
        }

		u32 now = device->getTimer()->getTime();
		game.setElapsedTime((f32)(now - then) / 1000.f); // Time in seconds
		then = now;

		driver->beginScene(true, true, 0);
		game.getSceneManager()->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
}
