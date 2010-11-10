#ifndef ITEM_H
#define ITEM_H
#include <string>

#define DEFAULT_ITEM_LEVEL 1

using namespace std;

class Item {
    private:
        int level_;
        int ID_;
        string name_;
        static int id_;
        int generateID();

    protected:

    public:
        int getLevel();
        int getID();
        string getName();

        Item(string name, int level = DEFAULT_ITEM_LEVEL);
        virtual ~Item();

};

#endif // ITEM_H
