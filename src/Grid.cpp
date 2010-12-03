#include "Grid.h"
#include "Level.h"

void Grid::fillCell(position2di position, bool occupied) {
    grid_[position.X][position.Y].occupy(occupied);
}

void Grid::fillCell(position2di position, Item* item) {
    grid_[position.X][position.Y].putItem(item);
}

void Grid::fillCell(position2di position, bool occupied, Item* item){
    fillCell(position, occupied);
    fillCell(position, item);
}

void Grid::mapTerrain(Level* level) {
    int sizeY = level->getSize().Height;
    int sizeX = level->getSize().Width;

    grid_ = new Cell*[sizeY];
    for (int i = 0; i < sizeY; i++)
        grid_[i] = new Cell[sizeX];
}

Grid::Grid() {
}

Grid::~Grid() {
}

