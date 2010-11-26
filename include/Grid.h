#ifndef _GRID_H
#define	_GRID_H

#include "Cell.h"
#include "Level.h"
#include <irrlicht/irrlicht.h>

using namespace irr::core;

class Grid {
    private:
        Cell** grid_;

    protected:


    public:
        void mapTerrain(Level* level);
        void mapObstacles(Level* level);
        void mapObstables(Character* character);

        Grid();
        Grid(const Grid& orig);
        virtual ~Grid();


};

#endif	/* _GRID_H */

