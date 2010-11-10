#include "Item.h"

int Item::id_ = 0;

int Item::generateID() {
    return id_++;
}

int Item::getLevel() {
    return level_;
}

int Item::getID() {
    return ID_;
}

string Item::getName() {
    return name_;
}

Item::Item(string name, int level) {
    level_ = level;
    ID_    = generateID();
    name_  = name;
}

Item::~Item() {
    //dtor
}
