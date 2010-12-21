#include "Slot.h"

Item * Slot::putItem(Item * item) {
    Item * oldItem = getItem();
    item_ = item;
    cout << "putei item" << endl;
    return oldItem;
}

Item * Slot::getItem() {
    return item_;
}

bool Slot::isEmpty() {
    return (item_ == NULL);
}

void Slot::clear() {
    //delete item_;
    item_ = NULL;
}

Slot::Slot() {
    item_ = NULL;
}

Slot::~Slot() {
    //dtor
}
