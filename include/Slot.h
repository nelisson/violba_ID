#ifndef SLOT_H
#define SLOT_H

#include "Item.h"

class Slot {
    private:
        Item * item_;

    protected:

    public:
        Item * putItem(Item * item);
        Item * getItem();
        bool isEmpty();

        void clear();

        Slot();
        virtual ~Slot();
};

#endif // SLOT_H
