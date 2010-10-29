#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"

enum weightClass{light, normal, heavy};

class Armor : public Item {
    private:
        int amount;
        weightClass type;

    protected:

    public:

        Armor(int level, string name);
        virtual ~Armor();
};

#endif // ARMOR_H
