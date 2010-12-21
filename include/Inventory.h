#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Slot.h"
#include "Potion.h"
#include <irrlicht/irrlicht.h>


#define DEFAULT_MAX_ITEMS 25
using namespace irr;
using namespace gui;
using namespace core;

class Inventory {
    private:
        vector<Slot*> slots_;
        int maxItems_;
        ISceneManager* sceneManager_;

    protected:

    public:

        void drawInventory();
        void setMaxItems(int maxItems);
        void increaseSize(int size);
        Item * removeItem(int index);
        Item * removeItem(vector<Slot*>::const_iterator i);
        void putItem(Item * item);
        Item * putItem(Item * item, int index);
        Potion* getPotion();

        Slot* findEmptySlot() const;

        void clear();

        Inventory(ISceneManager* sceneManager, int maxItems = DEFAULT_MAX_ITEMS);
        virtual ~Inventory();
};

#endif // INVENTORY_H
