#include <vector>

#include "ItemGenerator.h"

Item ItemGenerator::dropItem(int chance) {
    if ( randomBetween(0,100) < chance ) {

        cout << "VectorSize: " << items_.size() << endl;

        int random = randomBetween(0, items_.size() - 1);
        cout << "VectorIndex: " << random << endl;

        return items_[random];
    }
    else {
        cout << "vo larga excecao" << endl;
        throw new exception;
    }
}

ItemGenerator::ItemGenerator() {
}

ItemGenerator::~ItemGenerator() {
}

