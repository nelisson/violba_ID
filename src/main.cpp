#include <irrlicht.h>
#include <math.h>
#include "driverChoice.h"
#include "InputReceiver.h"
#include "Utils.h"


using namespace irr;
using namespace scene;
using namespace std;

int main() {
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		return 1;

	InputReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(800, 600), 32, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	core::array<SJoystickInfo> joystickInfo;
	device->activateJoysticks(joystickInfo);

	core::stringw tmp = L"Irrlicht Joystick Example (";
	tmp += joystickInfo.size();
	tmp += " joysticks)";
	device->setWindowCaption(tmp.c_str());

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	scene::EMD2_ANIMATION_TYPE playerAnim;

	//scene::IAnimatedMeshSceneNode* node =
	//	smgr->addAnimatedMeshSceneNode(smgr->getMesh("./models/ninja.b3d"));

   scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("./models/teste.md2"));

    if (node) {
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		//node->setFrameLoop(0, 59);
		node->setAnimationSpeed(30);
		node->setLoopMode(false);
		node->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}

//	scene::ISceneNode * plane = smgr->addCubeSceneNode();
//	if (node)
//	{
//        plane->setPosition(core::vector3df(0, -5, 20));
//        plane->setScale(core::vector3df(8, 0.01, 8));
//		plane->setMaterialTexture(0, driver->getTexture("./models/wall.bmp"));
//		plane->setMaterialFlag(video::EMF_LIGHTING, false);
//	}

    scene::ISceneNode * light = smgr->addLightSceneNode();


	scene::ICameraSceneNode * camera = smgr->addCameraSceneNode();

	// As in example 04, we'll use framerate independent movement.
	u32 then = device->getTimer()->getTime();
	const f32 MOVEMENT_SPEED = 10.f;

	while(device->run()) {
        camera->setPosition(node->getPosition()-core::vector3df(0, -10, 30));

	    // Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		bool movedWithJoystick = false;
		core::vector3df nodePosition = node->getPosition();
        core::vector3df nodeRotation = node->getRotation();

        //camera->setTarget(nodePosition);

		if(joystickInfo.size() > 0) {
			f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
			f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.

            f32 rotateHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
			f32 rotateVertical = 0.f; // -1.f for full down to +1.f for full up.

			const SEvent::SJoystickEvent & joystickData = receiver.GetJoystickState();

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

           if(joystickData.IsButtonPressed(0)){

               node->setMD2Animation(scene::EMAT_RUN);
           }
            if(joystickData.IsButtonPressed(1)){
               node->setMD2Animation(scene::EMAT_DEATH_FALLBACKSLOW);
           }
           if(joystickData.IsButtonPressed(2)){
               node->setMD2Animation(scene::EMAT_DEATH_FALLFORWARD);
           }
			// POV hat info is only currently supported on Windows, but the value is
			// guaranteed to be 65535 if it's not supported, so we can check its range.
			const u16 povDegrees = joystickData.POV / 100;

			if(povDegrees < 360) {
				if(povDegrees > 0 && povDegrees < 180)
					moveHorizontal = 1.f;
				else if(povDegrees > 180)
					moveHorizontal = -1.f;

				if(povDegrees > 90 && povDegrees < 270)
					moveVertical = -1.f;
				else if(povDegrees > 270 || povDegrees < 90)
					moveVertical = +1.f;
			}

			if(!core::equals(rotateHorizontal, 0.f) || !core::equals(rotateVertical, 0.f)) {
			}

			if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f)) {
			    double ang = sinal(moveHorizontal) *(180.0/3.1415)*acos(core::vector3df(0,0,1).dotProduct(core::vector3df(moveHorizontal,0,moveVertical).normalize()));

                //cout << "ang " << ang << endl;
			    node->setRotation(core::vector3df(0,ang,0));
			    nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
				nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime * moveVertical;

				movedWithJoystick = true;
			}
		}


		node->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255,113,113,133));
		smgr->drawAll(); // draw the 3d scene
		driver->endScene();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}

/*
**/
