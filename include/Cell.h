#ifndef _CELL_H
#define	_CELL_H

#include "Item.h"

class Cell {
    private:
        Item* item_;
        bool occupied_;

    protected:
        
    public:
        void occupy(bool occupation) { occupied_ = occupation; }

        bool isOccupied() const { return occupied_; }
        bool hasItem() const { return item_; }
        bool isEmpty() const { return !isOccupied() && !hasItem(); }

        Item* getItem() const { return item_; }
        void putItem(Item* item) { item_ = item; }
        void removeItem() { item_ = NULL; }

        Cell();
        virtual ~Cell();
};

#endif	/* _CELL_H */

