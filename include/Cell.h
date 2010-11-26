#ifndef _CELL_H
#define	_CELL_H

#include "Item.h"

class Cell {
    private:
        Item* item_;
        bool occupied_;

    protected:
        
    public:
        void occupy() { occupied_ = true; }
        void disoccupy() { occupied_ = false; }

        bool isOccupied() { return occupied_; }
        bool hasItem() { return item_; }
        bool isEmpty() { return !isOccupied() && !hasItem(); }

        Item* getItem() { return item_; }
        void putItem(Item* item) { item_ = item; }
        void removeItem() { item_ = NULL; }

        Cell();
        virtual ~Cell();
};

#endif	/* _CELL_H */

