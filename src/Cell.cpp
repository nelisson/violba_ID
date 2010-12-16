#include "Cell.h"

Cell::Cell(position2di position)
    : position_(position) {
    removeItem();
}

Cell::~Cell() {}

