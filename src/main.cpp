#include <irrlicht.h>
#include <math.h>
#include "driverChoice.h"
#include "XBOX360Controller.h"
#include "Utils.h"
#include <cstdlib>
#include <ctime>

#include "MainCharacter.h"
#include "Monster.h"

#define RESOLUTION core::dimension2d<u32>(800, 600)

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
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		return 1;

    srand(time(NULL));

	XBOX360Controller receiver;

    cout << "funcao: " << driverType << endl;
    cout << "OPENGL: " << video::EDT_OPENGL << endl;

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
			RESOLUTION, 32, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	core::array<SJoystickInfo> joystickInfo;
	device->activateJoysticks(joystickInfo);

	//cout << joystickInfo[0].Name.c_str() << endl;

	core::stringw tmp = L"Violba_ID (";
	tmp += joystickInfo.size();
	tmp += " joysticks)";
	device->setWindowCaption(tmp.c_str());

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	scene::EMD2_ANIMATION_TYPE playerAnim;

    MainCharacter * player = new MainCharacter("Violba", "./models/ninja.b3d");

	scene::IAnimatedMeshSceneNode* nodePlayer =
		smgr->addAnimatedMeshSceneNode(smgr->getMesh(player->getModelPath()));

    if (nodePlayer) {
        StopAnimationWhenStop stopStop;
        //nodePlayer->setAnimationEndCallback(dynamic_cast<IAnimationEndCallBack*> (&stopStop));
        nodePlayer->OnAnimate(0);
		nodePlayer->setMaterialFlag(video::EMF_LIGHTING, false);
		//node->setFrameLoop(0, 59);
		nodePlayer->setAnimationSpeed(30);
		nodePlayer->setLoopMode(false);
		//nodePlayer->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
		nodePlayer->setMaterialFlag(video::EMF_LIGHTING, false);
	}

    vector<Monster*> monsters;
    vector<int> delays;
    vector<scene::IAnimatedMeshSceneNode*> nodes;

    scene::IAnimatedMeshSceneNode* nodeMonster;


    scene::ISceneNode * plane = NULL;
	//scene::ISceneNode * plane = smgr->addCubeSceneNode();
	if (plane) {
        plane->setPosition(core::vector3df(0, -5, 20));
        plane->setScale(core::vector3df(8, 0.01, 8));
		plane->setMaterialTexture(0, driver->getTexture("./models/wall.bmp"));
		plane->setMaterialFlag(video::EMF_LIGHTING, false);
	}

    scene::ISceneNode * light = smgr->addLightSceneNode();


	scene::ICameraSceneNode * camera = smgr->addCameraSceneNode();

	// As in example 04, we'll use framerate independent movement.
	u32 then = device->getTimer()->getTime();
	const f32 MOVEMENT_SPEED = 10.f;

	while(device->run()) {
        camera->setPosition(nodePlayer->getPosition() - core::vector3df(0, -10, 30));

        vector<Monster*>::iterator i_monster;
        vector<int>::iterator i_delays;
        vector<scene::IAnimatedMeshSceneNode*>::iterator i_nodes;
        for (i_monster = monsters.begin(), i_delays = delays.begin(), i_nodes = nodes.begin(); i_monster < monsters.end(); i_monster++, i_delays++, i_nodes++) {
            if (--(*i_delays) == 0) {
                delays.erase(i_delays);
                //delete *i_monster;
                monsters.erase(i_monster);

                (*i_nodes)->remove();
                //delete *i_nodes;
                nodes.erase(i_nodes);
            }
            else
                if (nodePlayer->getPosition().getDistanceFrom((*i_nodes)->getPosition()) > 5) {
                    core::vector3df vetor = nodePlayer->getPosition() - (*i_nodes)->getPosition();
                    vetor.normalize();
                    (*i_nodes)->setPosition( (*i_nodes)->getPosition() + (vetor * 0.2) );
                }
        }

        int random = rand() % 1000;
        if (random < 10) {
            monsters.push_back(new Monster("Cubo da morte", "./models/teste.md2", 100, 10, 1));
            delays.push_back(1000);
            int randomx = (rand() % 100) - 50;
            int randomz = (rand() % 100) - 50;

            nodes.push_back(nodeMonster = smgr->addAnimatedMeshSceneNode(smgr->getMesh(monsters.back()->getModelPath())));
            nodeMonster->OnAnimate(0);
            nodeMonster->setPosition(core::vector3df(randomx, 0.0, randomz));

            if (nodeMonster) {
                nodeMonster->setMaterialFlag(video::EMF_LIGHTING, false);
                //node->setFrameLoop(0, 59);
                nodeMonster->setAnimationSpeed(30);
                nodeMonster->setLoopMode(false);
                nodeMonster->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
                nodeMonster->setMaterialFlag(video::EMF_LIGHTING, false);
            }

        }

	    // Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		bool movedWithJoystick = false;
		core::vector3df nodePosition = nodePlayer->getPosition();
        core::vector3df nodeRotation = nodePlayer->getRotation();

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

            if(moveHorizontal==0 && moveVertical==0){
                if(idle){
                    idle = false;
                    nodePlayer->setFrameLoop(205, 249);
                }
            }

	        if(joystickData.IsButtonPressed(0) && !globalSacana){
                nodePlayer->setFrameLoop(44, 58);
                globalSacana = true;
            }
            if(joystickData.IsButtonPressed(1) && !globalSacana){
                nodePlayer->setFrameLoop(31, 43);
                globalSacana = true;

            }
            if(joystickData.IsButtonPressed(2) && !globalSacana){
                nodePlayer->setFrameLoop(72,82);
                globalSacana = true;

            }
            if(joystickData.IsButtonPressed(3) && !globalSacana){
                nodePlayer->setFrameLoop(59,67);
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

			    nodePlayer->setRotation(core::vector3df(0,ang,0));
			    nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
				nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime * moveVertical;

				movedWithJoystick = true;
			}
		}


		nodePlayer->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255,113,113,133));
		smgr->drawAll(); // draw the 3d scene
		driver->endScene();
	}

	device->drop();

	return 0;
}
