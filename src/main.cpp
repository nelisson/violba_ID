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

bool globalSacana = false;
bool idle = true;

class StopAnimationWhenStop : public IAnimationEndCallBack{

    public:
        void OnAnimationEnd(IAnimatedMeshSceneNode *node) {
            node->setFrameLoop(205, 249);
            globalSacana = false;
            idle = true;
        }
};

int main() {

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
			RESOLUTION, 32, false, false, false);

    Game game(device->getSceneManager());
    device->setEventReceiver(&game.getController());

    MainCharacter ninja          = game.getMainCharacter();
    ICameraSceneNode * camera    = game.getCameras()[0];
    ILightSceneNode * light      = game.getLights()[0];
    XBOX360Controller controller = game.getController();

	core::array<SJoystickInfo> joystickInfo;
	device->activateJoysticks(joystickInfo);

    core::stringw programName = PROGRAM_NAME;
	device->setWindowCaption(programName.c_str());

	video::IVideoDriver* driver = device->getVideoDriver();

	scene::EMD2_ANIMATION_TYPE playerAnim;

    StopAnimationWhenStop stopStop;
    //nodePlayer->setAnimationEndCallback(dynamic_cast<IAnimationEndCallBack*> (&stopStop));
    ninja.getNode()->OnAnimate(0);
    ninja.getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
    //node->setFrameLoop(0, 59);
    ninja.getNode()->setAnimationSpeed(30);
    ninja.getNode()->setLoopMode(false);
    //nodePlayer->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
    ninja.getNode()->setMaterialFlag(video::EMF_LIGHTING, false);

    vector<int> delays;

/*
    scene::ISceneNode * plane = NULL;
	//scene::ISceneNode * plane = smgr->addCubeSceneNode();
	if (plane) {
        plane->setPosition(core::vector3df(0, -5, 20));
        plane->setScale(core::vector3df(8, 0.01, 8));
		plane->setMaterialTexture(0, driver->getTexture("./models/wall.bmp"));
		plane->setMaterialFlag(video::EMF_LIGHTING, false);
	}
*/

	u32 then = device->getTimer()->getTime();
	const f32 MOVEMENT_SPEED = 10.f;

	while(device->run()) {
        camera->setPosition(ninja.getNode()->getPosition() - core::vector3df(0, -10, 30));

        const SEvent::SJoystickEvent & joystickData = controller.GetJoystickState();
        //controller.OnEvent(joystickData);

        vector<Monster>::iterator i_monster;
        vector<int>::iterator i_delays;
        for (i_monster = game.getMonsters().begin(), i_delays = delays.begin(); i_monster < game.getMonsters().end(); i_monster++, i_delays++) {
            if (--(*i_delays) == 0) {
                delays.erase(i_delays);
                game.getMonsters().erase(i_monster);
            }
            else
                if (ninja.getNode()->getPosition().getDistanceFrom(i_monster->getNode()->getPosition()) > i_monster->getRange()) {
                    core::vector3df vetor = ninja.getNode()->getPosition() - i_monster->getNode()->getPosition();
                    vetor.normalize();
                    i_monster->getNode()->setPosition( i_monster->getNode()->getPosition() + (vetor * 0.2) );
                }
        }

        int random = rand() % 1000;
        if (random < 10) {
            Monster newMonster = Monster("Cubo da morte", "./models/teste.md2", 100, 10, 1);
            game.addMonster(newMonster);
            delays.push_back(1000);
            int randomx = (rand() % 100) - 50;
            int randomz = (rand() % 100) - 50;

            IAnimatedMeshSceneNode * currentMonsterNode = game.getMonsters().back().getNode();




            cout << (currentMonsterNode == NULL ? "null" : "nao") << endl;





            currentMonsterNode->OnAnimate(0);
            currentMonsterNode->setPosition(core::vector3df(randomx, 0.0, randomz));
            currentMonsterNode->setMaterialFlag(video::EMF_LIGHTING, false);
            //node->setFrameLoop(0, 59);
            currentMonsterNode->setAnimationSpeed(30);
            currentMonsterNode->setLoopMode(false);
            currentMonsterNode->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
            currentMonsterNode->setMaterialFlag(video::EMF_LIGHTING, false);
        }

	    // Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		bool movedWithJoystick = false;
		core::vector3df nodePosition = ninja.getNode()->getPosition();
        core::vector3df nodeRotation = ninja.getNode()->getRotation();

        //camera->setTarget(nodePosition);

        f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
        f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.

        f32 rotateHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
        f32 rotateVertical = 0.f; // -1.f for full down to +1.f for full up.


        // We receive the full analog range of the axes, and so have to implement our
        // own dead zone.  This is an empirical value, since some joysticks have more
        // jitter or creep around the center point than others.  We'll use 5% of the
        // range as the dead zone, but generally you would want to give the user the
        // option to change this.
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
                ninja.getNode()->setFrameLoop(205, 249);
            }
        }

        if(joystickData.IsButtonPressed(0) && !globalSacana){
            ninja.getNode()->setFrameLoop(44, 58);
            globalSacana = true;
        }
        if(joystickData.IsButtonPressed(1) && !globalSacana){
            ninja.getNode()->setFrameLoop(31, 43);
            globalSacana = true;

        }
        if(joystickData.IsButtonPressed(2) && !globalSacana){
            ninja.getNode()->setFrameLoop(72,82);
            globalSacana = true;

        }
        if(joystickData.IsButtonPressed(3) && !globalSacana){
            ninja.getNode()->setFrameLoop(59,67);
            globalSacana = true;

        }
//
//			moveHorizontal =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
//			if(fabs(moveHorizontal) < DEAD_ZONE)
//				moveHorizontal = 0.f;
//
//			moveVertical =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
//			if(fabs(moveVertical) < DEAD_ZONE)
//				moveVertical = 0.f;
//
//			rotateHorizontal =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;
//			if(fabs(rotateHorizontal) < DEAD_ZONE)
//				rotateHorizontal = 0.f;
//
//			rotateVertical =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_R] / -32767.f;
//			if(fabs(rotateVertical) < DEAD_ZONE)
//				rotateVertical = 0.f;
//
//           if(joystickData.IsButtonPressed(0)){
//
//               nodePlayer->setMD2Animation(scene::EMAT_RUN);
//           }
//            if(joystickData.IsButtonPressed(1)){
//               nodePlayer->setMD2Animation(scene::EMAT_DEATH_FALLBACKSLOW);
//           }
//           if(joystickData.IsButtonPressed(2)){
//               nodePlayer->setMD2Animation(scene::EMAT_DEATH_FALLFORWARD);
//           }
//			// POV hat info is only currently supported on Windows, but the value is
//			// guaranteed to be 65535 if it's not supported, so we can check its range.
//			const u16 povDegrees = joystickData.POV / 100;
//
//			if(povDegrees < 360) {
//				if(povDegrees > 0 && povDegrees < 180)
//					moveHorizontal = 1.f;
//				else if(povDegrees > 180)
//					moveHorizontal = -1.f;
//
//				if(povDegrees > 90 && povDegrees < 270)
//					moveVertical = -1.f;
//				else if(povDegrees > 270 || povDegrees < 90)
//					moveVertical = +1.f;
//			}

        if(!core::equals(rotateHorizontal, 0.f) || !core::equals(rotateVertical, 0.f)) {
        }

        if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f)) {
            double ang = sinal(moveHorizontal) *(180.0/3.1415)*acos(core::vector3df(0,0,1).dotProduct(core::vector3df(moveHorizontal,0,moveVertical).normalize()));

            ninja.getNode()->setRotation(core::vector3df(0,ang,0));
            nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
            nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime * moveVertical;

            movedWithJoystick = true;
        }


		ninja.getNode()->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255,113,113,133));
		game.getSceneManager()->drawAll(); // draw the 3d scene
		driver->endScene();
	}

	device->drop();

	return 0;
}
