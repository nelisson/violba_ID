#include "Level.h"

float Level::LeastCostEstimate(void* stateStart, void* stateEnd) {
    return 1;
}

void Level::AdjacentCost(void* state, vector<StateCost>* adjacent) {

}

Level::Level(ISceneManager * manager)
    : ISceneNode(manager->getRootSceneNode(), manager) {

    IVideoDriver * driver = getSceneManager()->getVideoDriver();

    terrain_ = getSceneManager()->addTerrainSceneNode("./models/heightmap.bmp",
                                                      getSceneManager()->getRootSceneNode(),					// parent node
                                                      -1,					// node id
                                                      core::vector3df(-1.f, -10.f,   -800.f),		// position
                                                      core::vector3df(0.f, 0.f, 0.f),		// rotation
                                                      core::vector3df(40.f, 3.0f, 40.f),	// scale
                                                      video::SColor ( 255, 255, 255, 255 ),	// vertexColor
                                                      5,					// maxLOD
                                                      scene::ETPS_17,				// patchSize
                                                      4);				// smoothFactor

    terrain_->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain_->setMaterialTexture(0, driver->getTexture("./models/terrain-texture.jpg"));
    //terrain_->setMaterialTexture(1, driver->getTexture("./models/detailmap3.jpg"));
    terrain_->setMaterialType(video::EMT_DETAIL_MAP);
    terrain_->scaleTexture(10.0f, 150.0f);

    cout << "Terrain X :" << terrain_->getTerrainCenter().X << " Z: " << terrain_->getTerrainCenter().Z << endl;
    cout << "Pos X :" << terrain_->getPosition().X << " Z: " << terrain_->getPosition().Z << endl;


    size_ = dimension2df(getPosition().X + terrain_->getTerrainCenter().X, getPosition().Z + terrain_->getTerrainCenter().Z);
    selector_ = getSceneManager()->createTerrainTriangleSelector(getTerrain());

    setAutomaticCulling(irr::scene::EAC_OFF);
}

Level::~Level() {
    remove();
}
