#ifndef _CELL_H
#define	_CELL_H

#include "Item.h"

using namespace irr;
using namespace irr::core;

class Cell {
    private:
        Item* item_;
        bool occupied_;
        position2di position_;

    protected:
        
    public:
        void occupy(bool occupation) { occupied_ = occupation; }

        void setPosition(position2di position) { position_ = position; }
        const position2di getPosition() const { return position_; }

        bool isOccupied() const { return occupied_; }
        bool hasItem() const { return item_; }
        bool isEmpty() const { return !isOccupied() && !hasItem(); }

        Item* getItem() const { return item_; }
        void putItem(Item* item) { item_ = item; }
        void removeItem() { item_ = NULL; }

        void clear() { removeItem(); occupy(false); }

        Cell() {}
        Cell(position2di position);
        virtual ~Cell();
};

#endif	/* _CELL_H */

