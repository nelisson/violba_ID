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



    ITerrainSceneNode* terrain = game.getSceneManager()->addTerrainSceneNode(
		"./models/heightmap.bmp",
		0,					// parent node
		-1,					// node id
		core::vector3df(-300.f, -10.f, -100.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(40.f, 4.4f, 40.f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0,
			driver->getTexture("./models/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,
			driver->getTexture("./models/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);

	CDynamicMeshBuffer* buffer = new CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
	terrain->getMeshBufferForLOD(*buffer, 0);
	video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();

	buffer->drop();

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	ISceneNode* skybox= game.getSceneManager()->addSkyBoxSceneNode(
		driver->getTexture("./models/irrlicht2_up.jpg"),
		driver->getTexture("./models/irrlicht2_dn.jpg"),
		driver->getTexture("./models/irrlicht2_lf.jpg"),
		driver->getTexture("./models/irrlicht2_rt.jpg"),
		driver->getTexture("./models/irrlicht2_ft.jpg"),
		driver->getTexture("./models/irrlicht2_bk.jpg"));
	ISceneNode* skydome=game.getSceneManager()->addSkyDomeSceneNode(driver->getTexture("./models/skydome.jpg"),16,8,0.95f,2.0f);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);




	while(device->run()) {

	    core::vector3df camerapos = game.getCameras().at(0)->getPosition();
	    core::vector3df ninjapos = ninja->getNode()->getPosition();

	    cout << "Camera X: " << camerapos.X;
	    cout << " Y: " << camerapos.Y;
	    cout << " Z: " << camerapos.Z << endl;

	    cout << "Ninja X: " << ninjapos.X;
	    cout << " Y: " << ninjapos.Y;
	    cout << " Z: " << ninjapos.Z << endl;

/*
        for (int i = 0; i < monsters->size(); i++) {
            core::position2df position = monsters->at(i)->getNode()->getBoun
            driver->draw2DLine()
*/

        game.doActions();
        // cubos

        int random = rand() % 1000;
        if (random < 10) {
            Monster * newMonster = new Monster("Cubo da morte", "./models/dwarf.x", 100, 10, 1);

            game.addMonster(newMonster);

            int randomx = (rand() % 100) - 50;
            int randomz = (rand() % 100) - 50;

            newMonster->getNode()->OnAnimate(0);
            newMonster->getNode()->setPosition(core::vector3df(randomx, 0.0, randomz));
            newMonster->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
            newMonster->getNode()->setAnimationSpeed(30);
            newMonster->getNode()->setLoopMode(false);
            newMonster->getNode()->setScale(core::vector3df(0.1, 0.1, 0.1));
            //newMonster->getNode()->setMaterialTexture(0, driver->getTexture("./models/water.jpg"));
            newMonster->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        for (int i = 0; i < monsters->size(); i++) {
            if (ninja->getNode()->getPosition().getDistanceFrom(monsters->at(i)->getNode()->getPosition()) > monsters->at(i)->getRange()) {
                core::vector3df vetor = ninja->getNode()->getPosition() - monsters->at(i)->getNode()->getPosition();
                vetor.normalize();
                monsters->at(i)->getNode()->setPosition( monsters->at(i)->getNode()->getPosition() + (vetor * 0.2) );

                core::vector3df dwarfToNinja = ninja->getNode()->getPosition() - monsters->at(i)->getNode()->getPosition();
                float angle = 90 + 180/3.14 * acos(dwarfToNinja.normalize().dotProduct(core::vector3df(1.0, 0.0, 0.0)));
                monsters->at(i)->getNode()->setRotation( core::vector3df(0.0, -angle, 0.0));
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
