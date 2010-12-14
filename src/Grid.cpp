#include "Grid.h"
#include "Level.h"

void Grid::fillCell(position2di position, bool occupied) {
    grid_[position.X][position.Y].occupy(occupied);
}

void Grid::fillCell(position2di position, Item* item) {
    cout<<"X: "<<position.X<<" Y: "<< position.Y<<endl;

    cout<<"XSize: "<<sizeX_<<" YSize: "<< sizeY_<<endl;

    item->setPosition( vector3df(position.X, 10, position.Y) ) ;
    grid_[position.X][position.Y].putItem(item);
}

void Grid::fillCell(position2di position, bool occupied, Item* item){
    fillCell(position, occupied);
    fillCell(position, item);
}

void Grid::mapTerrain(Level* level) {
    sizeY_ = level->getSize().Height;
    sizeX_ = level->getSize().Width;

    grid_ = new Cell*[sizeY_];
    for (int i = 0; i < sizeY_; i++)
        grid_[i] = new Cell[sizeX_];
}

Grid::Grid(Level* level) {
    mapTerrain(level);
}

Grid::~Grid() {
}

