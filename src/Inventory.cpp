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

Item * Inventory::putItem(Item * item) {
    //if ()
}

Item * Inventory::putItem(Item *, int) {

}

Inventory::Inventory(int maxItems) {
    for (int i = 0; i < maxItems_; i++)
        slots_.push_back(new Slot());
}

Inventory::~Inventory() {
    //dtor
}
