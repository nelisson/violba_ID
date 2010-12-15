#include <vector>

#include "ItemGenerator.h"

void ItemGenerator::loadWeapons() {
    items_.push_back(Weapon(NULL, NULL, "Arma 1"));
    items_.push_back(Weapon(NULL, NULL, "Arma 2"));
    items_.push_back(Weapon(NULL, NULL, "Arma 3"));
    items_.push_back(Weapon(NULL, NULL, "Arma 4"));
}

void ItemGenerator::loadArmors() {
//    items_.push_back(Armor(NULL, NULL, "Armor 1"));
//    items_.push_back(Armor(NULL, NULL, "Armor 2"));
//    items_.push_back(Armor(NULL, NULL, "Armor 3"));
}

void ItemGenerator::createItems() {
    loadWeapons();
    loadArmors();
}

Item ItemGenerator::dropItem(int chance) {
    if ( randomBetween(0,100) <= chance )
        return items_[randomBetween(0, items_.size() - 1)];
    else
        throw 1;
}

ItemGenerator::ItemGenerator() {
    createItems();
}

ItemGenerator::~ItemGenerator() {
}

