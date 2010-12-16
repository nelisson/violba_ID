#include "Level.h"

float Level::LeastCostEstimate(void* stateStart, void* stateEnd) {
    return 1;
}

void Level::AdjacentCost(void* state, vector<StateCost>* adjacent) {

}

Level::Level(ISceneManager * manager)
    : ISceneNode(manager->getRootSceneNode(), manager) {

    IVideoDriver * driver = getSceneManager()->getVideoDriver();

    //"./models/heightmap.bmp"
    terrain_ = getSceneManager()->addTerrainSceneNode("./models/heightmapwow.bmp",
                                                      getSceneManager()->getRootSceneNode(),					// parent node
                                                      -1,					// node id
                                                      core::vector3df(0.f, 0.f, 0.f),		// position
                                                      core::vector3df(0.f, 0.f, 0.f),		// rotation
                                                      core::vector3df(1.f, 1.0f, 1.f),	// scale
                                                      video::SColor ( 255, 255, 255, 255 ),	// vertexColor
                                                      50,					// maxLOD
                                                      scene::ETPS_17,				// patchSize
                                                      1);				// smoothFactor

    terrain_->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain_->setMaterialTexture(0, driver->getTexture("./models/terrain-texture.jpg"));
    terrain_->setMaterialTexture(1, driver->getTexture("./models/detailmap3.jpg"));
    terrain_->setMaterialType(video::EMT_DETAIL_MAP);
    terrain_->getMaterial(0).TextureLayer[0].AnisotropicFilter = 16;
    terrain_->setScale(vector3df(10,0.4,10));
    terrain_->scaleTexture(10);

    cout << "Terrain X :" << terrain_->getTerrainCenter().X << " Z: " << terrain_->getTerrainCenter().Z << endl;
    cout << "Pos X :" << terrain_->getPosition().X << " Z: " << terrain_->getPosition().Z << endl;

    vector3df size = 2* (terrain_->getTerrainCenter() - terrain_->getPosition());
    size_ = dimension2df(size.X, size.Z);
    selector_ = getSceneManager()->createTerrainTriangleSelector(getTerrain());

    setAutomaticCulling(irr::scene::EAC_OFF);
}

Level::~Level() {
    remove();
}
