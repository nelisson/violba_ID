#include "Inventory.h"

void Inventory::setMaxItems(int maxItems) {
    maxItems_ = maxItems;
}

void Inventory::increaseSize(int size) {
    maxItems_ += size;
}

Item * Inventory::removeItem(int) {
    //slots_
}

void Inventory::putItem(Item * item) {
    try {
        findEmptySlot()->putItem(item);
        cout << "putei item fora" << endl;
    }
    catch (int i) {
        throw;
    }
}

Slot* Inventory::findEmptySlot() const {
    vector<Slot*>::const_iterator i;

    for (i = slots_.begin(); i < slots_.end(); i++) {
        cout << "for slots" << endl;
        printf("%p\n", *i);
        cout << endl;
        if ((*i)->isEmpty()) {
            cout << "is empty" << endl;
            return *i;
        }
        else {
            cout << "not empty" << endl;
        }
    }

    throw 2;
}

Item * Inventory::putItem(Item *, int) {

}

Inventory::Inventory(int maxItems)
    : maxItems_(maxItems) {

    for (int i = 0; i < maxItems_; i++) {
        slots_.push_back(new Slot());
        printf("%p\n", slots_[i]);
    }
}

Inventory::~Inventory() {
    //dtor
}
