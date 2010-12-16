#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Slot.h"

#define DEFAULT_MAX_ITEMS 2

class Inventory {
    private:
        vector<Slot*> slots_;
        int maxItems_;

    protected:

    public:
        void setMaxItems(int maxItems);
        void increaseSize(int size);
        Item * removeItem(int index);
        void putItem(Item * item);
        Item * putItem(Item * item, int index);

        Slot* findEmptySlot() const;

        Inventory(int maxItems = DEFAULT_MAX_ITEMS);
        virtual ~Inventory();
};

#endif // INVENTORY_H
