#ifndef SLOT_H
#define SLOT_H

#include "item.h"

class Slot {
    private:
        Item * item_;

    protected:

    public:
        Item * putItem(Item * item);
        Item * getItem();
        bool isEmpty();

        Slot();
        virtual ~Slot();
};

#endif // SLOT_H
