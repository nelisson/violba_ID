#ifndef _GRID_H
#define	_GRID_H

#include "Cell.h"
#include "Level.h"
#include <irrlicht/irrlicht.h>

using namespace irr::core;

enum CellAttributes {
    ITEM = 1,
    CHARACTER = 1 << 1,
};

class Grid {
    private:
        Cell** grid_;

    protected:


    public:
        void mapTerrain(Level* level);
        void fillCell(position2di position, bool occupied);
        void fillCell(position2di position, Item* item);
        void fillCell(position2di position, bool occupied, Item* item);

        Grid();
        Grid(const Grid& orig);
        virtual ~Grid();


};

#endif	/* _GRID_H */

