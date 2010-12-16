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
        int sizeX_;
        int sizeY_;

    protected:


    public:
        void mapTerrain(Level* level);
        void fillCell(position2di position, bool occupied);
        void fillCell(position2di position, Item* item);
        void fillCell(position2di position, bool occupied, Item* item);

        bool hasItem(position2di position) { return grid_[position.X][position.Y].hasItem(); }
        Item* getItem(position2di position) { return grid_[position.X][position.Y].getItem(); }
        Item* getItem(recti block) const;
        vector<Item*> getItems(recti block) const;

        void clearItems(recti block);

        void clear();

        Grid() {};
        Grid(Level* level);
        virtual ~Grid();


};

#endif	/* _GRID_H */

