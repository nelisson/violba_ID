#ifndef _ITEMGENERATOR_H
#define	_ITEMGENERATOR_H

#include "Weapon.h"
#include "Armor.h"
#include <iostream>

using namespace std;

class ItemGenerator {
    private:
        vector<Item> items_;

    protected:

    public:
        void loadItem(Item item) { items_.push_back(item); }
        void loadItems(vector<Item> items) { items_.insert(items_.end(), items.begin(), items.end()); cout <<"Items count: " << items_.size() <<endl; }
        Item dropItem(int chance);

        ItemGenerator();
        virtual ~ItemGenerator();
};

#endif	/* _ITEMGENERATOR_H */

