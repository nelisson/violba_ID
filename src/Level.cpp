#include "Level.h"

float Level::LeastCostEstimate(void* stateStart, void* stateEnd) {
    return 1;
}

void Level::AdjacentCost(void* state, vector<StateCost>* adjacent) {

}

Level::Level(ISceneManager * manager)
    : ISceneNode(manager->getRootSceneNode(), manager, NodeIDFlags::IGNORED) {

    IVideoDriver * driver = getSceneManager()->getVideoDriver();

    //"./models/heightmap.bmp"
    terrain_ = getSceneManager()->addTerrainSceneNode("./models/heightmapwow.bmp",
                                                      getSceneManager()->getRootSceneNode(),					// parent node
                                                      NodeIDFlags::FLOOR,					// node id
                                                      core::vector3df(0.f, 0.f, 0.f),		// position
                                                      core::vector3df(0.f, 0.f, 0.f),		// rotation
                                                      core::vector3df(1.f, 1.0f, 1.f),	// scale
                                                      video::SColor ( 255, 255, 255, 255 ),	// vertexColor
                                                      50,					// maxLOD
                                                      scene::ETPS_17,				// patchSize
                                                      1);				// smoothFactor

    terrain_->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain_->setMaterialTexture(0, driver->getTexture("./models/Tatami.psd"));
    terrain_->setMaterialType(video::EMT_DETAIL_MAP);
    terrain_->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    terrain_->scaleTexture(16);

    cout << "Terrain X :" << terrain_->getTerrainCenter().X << " Z: " << terrain_->getTerrainCenter().Z << endl;
    cout << "Pos X :" << terrain_->getPosition().X << " Z: " << terrain_->getPosition().Z << endl;

    vector3df size = 2* (terrain_->getTerrainCenter() - terrain_->getPosition());
    size_ = dimension2df(size.X, size.Z);
    selector_ = getSceneManager()->createTerrainTriangleSelector(getTerrain());

    setAutomaticCulling(irr::scene::EAC_OFF);

    for(int i=1;i<24;i++){
        IMeshSceneNode* lado = getSceneManager()->addCubeSceneNode(1, 0, NodeIDFlags::IGNORED);

        lado->setScale(vector3df(0.2,25,25));
        lado->setPosition(vector3df(0,12.5,12.5*i));
        lado->setMaterialFlag(video::EMF_LIGHTING, false);
        lado->setMaterialTexture(0, driver->getTexture("./models/Wall1.tga"));
        lado->getMaterial(0).TextureLayer[0].AnisotropicFilter = 8;

    }

    for(int i=1;i<24;i++){
        IMeshSceneNode* lado = getSceneManager()->addCubeSceneNode(1, 0, NodeIDFlags::IGNORED);

        lado->setScale(vector3df(0.2,25,25));
        lado->setPosition(vector3df(getSize().Width,12.5,12.5*i));
        lado->setMaterialFlag(video::EMF_LIGHTING, false);
        lado->setMaterialTexture(0, driver->getTexture("./models/Wall1.tga"));
        lado->getMaterial(0).TextureLayer[0].AnisotropicFilter = 8;

    }

    for(int i=1;i<24;i++){
        IMeshSceneNode* lado = getSceneManager()->addCubeSceneNode(1, 0, NodeIDFlags::IGNORED);

        lado->setScale(vector3df(25,25,0.2));
        lado->setPosition(vector3df(12.5*i,12.5,getSize().Height));
        lado->setMaterialFlag(video::EMF_LIGHTING, false);
        lado->setMaterialTexture(0, driver->getTexture("./models/Wall1.tga"));
        lado->getMaterial(0).TextureLayer[0].AnisotropicFilter = 8;

    }
}

Level::~Level() {
    remove();
}
