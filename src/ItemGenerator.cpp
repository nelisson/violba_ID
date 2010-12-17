#include <vector>

#include "ItemGenerator.h"

void ItemGenerator::loadWeapons() {
    items_.push_back(new Weapon(NULL, NULL, "Arma 1"));
    items_.push_back(new Weapon(NULL, NULL, "Arma 2"));
}

void ItemGenerator::loadArmors() {
    items_.push_back(new Armor(NULL, NULL, "Armor"));
}

void ItemGenerator::loadPotions(){
    items_.push_back(new Potion(NULL, NULL, "Potion"));
}

void ItemGenerator::createItems() {
    loadWeapons();
    loadArmors();
    loadPotions();
}

Item* ItemGenerator::dropItem(int chance) {
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

