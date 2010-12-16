#include "Grid.h"
#include "Level.h"

void Grid::fillCell(position2di position, bool occupied) {
    grid_[position.X][position.Y].occupy(occupied);
}

void Grid::fillCell(position2di position, Item* item) {
    cout<<"X: "<<position.X<<" Y: "<< position.Y<<endl;

    cout<<"XSize: "<<sizeX_<<" YSize: "<< sizeY_<<endl;

    //item->setPosition( vector3df(position.X, 10, position.Y) ) ;
    grid_[position.X][position.Y].putItem(item);
}

void Grid::fillCell(position2di position, bool occupied, Item* item){
    fillCell(position, occupied);
    fillCell(position, item);
}

Item* Grid::getItem(recti block) const {
    for (int i = block.UpperLeftCorner.X; i < block.LowerRightCorner.X; i++)
        for (int j = block.UpperLeftCorner.Y; j < block.LowerRightCorner.Y; j++)
            if (grid_[i][j].hasItem()) {
                Item* item = grid_[i][j].getItem();
                return item;
            }

    throw 1;
}

vector<Item*> Grid::getItems(recti block) const {
    vector<Item*> result;

    cout << "-----Rect: \n";
    cout << "UpperLeft X: "<<block.UpperLeftCorner.X;
    cout << " UpperLeft Y: "<<block.UpperLeftCorner.Y;
    cout << " LowerRight X: "<<block.LowerRightCorner.X;
    cout << " LowerRight Y: "<<block.LowerRightCorner.Y <<endl;

    for (int i = block.UpperLeftCorner.X; i < block.LowerRightCorner.X; i++)
        for (int j = block.UpperLeftCorner.Y; j < block.LowerRightCorner.Y; j++)
            if (grid_[i][j].hasItem())
                result.push_back(grid_[i][j].getItem());

    if (result.size() > 0)
        return result;
    else
        throw 1;
}

void Grid::mapTerrain(Level* level) {
    sizeY_ = level->getSize().Height;
    sizeX_ = level->getSize().Width;

    grid_ = new Cell*[sizeY_];
    for (int i = 0; i < sizeY_; i++)
        grid_[i] = new Cell[sizeX_];
}

void Grid::clearItems(recti block) {
    for (int i = block.UpperLeftCorner.X; i < block.LowerRightCorner.X; i++)
        for (int j = block.UpperLeftCorner.Y; j < block.LowerRightCorner.Y; j++)
            grid_[i][j].removeItem();
}

void Grid::clear() {
    for (int i = 0; i < sizeX_; i++)
        for (int j = 0; j < sizeY_; j++)
            grid_[i][j].clear();
}

Grid::Grid(Level* level) {
    mapTerrain(level);
}

Grid::~Grid() {
}

