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
    ICameraSceneNode * camera    = game.getCameras()[0];
    ILightSceneNode * light      = game.getLights()[0];
    XBOX360Controller * controller = game.getController();
    //vector<Monster*> monsters = game.getMonsters();

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
    //nodePlayer->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
    ninja->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);

    vector<int> delays;

	u32 then = device->getTimer()->getTime();
	const f32 MOVEMENT_SPEED = 10.f;
    camera->setPosition(ninja->getNode()->getPosition() - core::vector3df(0, -10, 30));

	while(device->run()) {
        const SEvent::SJoystickEvent & joystickData = controller->GetJoystickState();
        //controller.OnEvent(joystickData);
        // cubos

        int random = rand() % 1000;
        if (random < 10) {
            Monster * newMonster = new Monster("Cubo da morte", "./models/teste.md2", 100, 10, 1);

            game.addMonster(newMonster);

            delays.push_back(1000);
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

        for (int i = 0; i < game.monsters_.size(); i++) {
            if (--(delays[i]) <= 0) {

                //game.monsters_.erase(game.monsters_.begin() + i);


                delays.erase(delays.begin() + i);
                delete game.monsters_.at(i);
                game.monsters_.erase(game.monsters_.begin() + i);


                break;
            }
            else
                if (ninja->getNode()->getPosition().getDistanceFrom(game.monsters_[i]->getNode()->getPosition()) > game.monsters_[i]->getRange()) {
                    core::vector3df vetor = ninja->getNode()->getPosition() - game.monsters_[i]->getNode()->getPosition();
                    vetor.normalize();
                    game.monsters_[i]->getNode()->setPosition( game.monsters_[i]->getNode()->getPosition() + (vetor * 0.2) );
                }
        }


		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
/*
		core::vector3df nodePosition = ninja->getNode()->getPosition();
        core::vector3df nodeRotation = ninja->getNode()->getRotation();

        f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
        f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.

        f32 rotateHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
        f32 rotateVertical = 0.f; // -1.f for full down to +1.f for full up.

        const f32 DEAD_ZONE = 0.20f;

        moveHorizontal =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
        if(fabs(moveHorizontal) < DEAD_ZONE)
            moveHorizontal = 0.f;

        moveVertical =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
        if(fabs(moveVertical) < DEAD_ZONE)
            moveVertical = 0.f;

        rotateHorizontal =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;
        if(fabs(rotateHorizontal) < DEAD_ZONE)
            rotateHorizontal = 0.f;

        rotateVertical =
            (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_R] / -32767.f;
        if(fabs(rotateVertical) < DEAD_ZONE)
            rotateVertical = 0.f;

        if(moveHorizontal==0 && moveVertical==0){
            if(idle){
                idle = false;
                ninja->getNode()->setFrameLoop(205, 249);
            }
        }

        if(joystickData.IsButtonPressed(0) && !globalSacana){
            ninja->getNode()->setFrameLoop(44, 58);
            globalSacana = true;
        }
        if(joystickData.IsButtonPressed(1) && !globalSacana){
            ninja->getNode()->setFrameLoop(31, 43);
            globalSacana = true;

        }
        if(joystickData.IsButtonPressed(2) && !globalSacana){
            ninja->getNode()->setFrameLoop(72,82);
            globalSacana = true;

        }
        if(joystickData.IsButtonPressed(3) && !globalSacana){
            ninja->getNode()->setFrameLoop(59,67);
            globalSacana = true;

        }

        if(!core::equals(rotateHorizontal, 0.f) || !core::equals(rotateVertical, 0.f)) {
        }

        if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f)) {
            double ang = sinal(moveHorizontal) *(180.0/3.1415)*acos(core::vector3df(0,0,1).dotProduct(core::vector3df(moveHorizontal,0,moveVertical).normalize()));

            ninja->getNode()->setRotation(core::vector3df(0,ang,0));
            nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
            nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime * moveVertical;

        }

		ninja->getNode()->setPosition(nodePosition);
*/
		driver->beginScene(true, true, 0);
		game.getSceneManager()->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
}
