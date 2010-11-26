#include "Grid.h"
#include "Level.h"

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

