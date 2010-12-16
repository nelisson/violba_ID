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

vector<Cell*> Grid::getItemCells(recti block) const {
    vector<Cell*> result;

    for (int i = block.UpperLeftCorner.X; i < block.LowerRightCorner.X; i++)
        for (int j = block.UpperLeftCorner.Y; j < block.LowerRightCorner.Y; j++)
            if (grid_[i][j].hasItem())
                result.push_back(&grid_[i][j]);

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

    for (int i = 0; i < sizeX_; i++)
        for (int j = 0; j < sizeY_; j++)
            grid_[i][j].setPosition(position2di(i,j));
}

void Grid::clearItems(recti block) {
    for (int i = block.UpperLeftCorner.X; i < block.LowerRightCorner.X; i++)
        for (int j = block.UpperLeftCorner.Y; j < block.LowerRightCorner.Y; j++)
            grid_[i][j].removeItem();
}

void Grid::AdjacentCost( void* state, vector<StateCost> *adjacent ) {
    adjacent = getNeighbors( ((Cell*) state)->getPosition() );
}

bool Grid::limitOk(position2di position) const {
    return (position.X >= 0 && position.X < sizeX_) &&
           (position.Y >= 0 && position.Y < sizeY_);
}

vector<StateCost>* Grid::getNeighbors(position2di position) const {
    vector<StateCost>* result = new vector<StateCost>();

    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            if ( limitOk(position2di(i,j) + position) && !(i == 0 && j == 0) ) {
                struct StateCost bla;
                bla.cost = 1;
                bla.state = (void*) getCell(position2di(i, j));
                result->push_back(bla);
            }

        //cout<<"Neigh size: " << result->size() << endl;

    return result;
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

